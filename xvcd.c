// xilinx virtual cable demon for jlink jtag
// use connection string in Impact and another
// xilinx_xvc host=localhost:2542 disableversioncheck=true

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <winsock2.h>

#include "io_jlink.h"
#include "JlinkUtil.h"

//
// Listen on port 2542.
//
#define DEFAULT_PORT 2542
#define DEFAULT_JTAG_SPEED 1000 // 1 Mhz

static int jtag_state;
static int verbose;

//
// JTAG state machine.
//

enum {
    test_logic_reset, run_test_idle,

    select_dr_scan, capture_dr, shift_dr,
    exit1_dr, pause_dr, exit2_dr, update_dr,

    select_ir_scan, capture_ir, shift_ir,
    exit1_ir, pause_ir, exit2_ir, update_ir,

    num_states
};

static int jtag_step(int state, int tms) {
  static const int next_state[num_states][2] =
      {
          [test_logic_reset] = {run_test_idle, test_logic_reset},
          [run_test_idle] = {run_test_idle, select_dr_scan},

          [select_dr_scan] = {capture_dr, select_ir_scan},
          [capture_dr] = {shift_dr, exit1_dr},
          [shift_dr] = {shift_dr, exit1_dr},
          [exit1_dr] = {pause_dr, update_dr},
          [pause_dr] = {pause_dr, exit2_dr},
          [exit2_dr] = {shift_dr, update_dr},
          [update_dr] = {run_test_idle, select_dr_scan},

          [select_ir_scan] = {capture_ir, test_logic_reset},
          [capture_ir] = {shift_ir, exit1_ir},
          [shift_ir] = {shift_ir, exit1_ir},
          [exit1_ir] = {pause_ir, update_ir},
          [pause_ir] = {pause_ir, exit2_ir},
          [exit2_ir] = {shift_ir, update_ir},
          [update_ir] = {run_test_idle, select_dr_scan}
      };

  return next_state[state][tms];
}

static int sread(SOCKET fd, void *target, int len) {
  char *t = target;
  while (len) {
    int r = recv(fd, t, len, 0);
    if (r == SOCKET_ERROR || r == 0) {
      int err = WSAGetLastError();
      printf( "Recv failed with error: %d\n", err );
      if(r == 0) {
        puts("Seems like connection was gracefully closed. Exiting");
      }
      return r;
    }
    t += r;
    len -= r;
  }
  return 1;
}

//
// handle_data(fd) handles JTAG shift instructions.
//   To allow multiple programs to access the JTAG chain
//   at the same time, we only allow switching between
//   different clients only when we're in run_test_idle
//   after going test_logic_reset. This ensures that one
//   client can't disrupt the other client's IR or state.
//
int handle_data(SOCKET fd) {
  int i;
  int seen_tlr = 0;

  do {
    char cmd[16];
    unsigned char buffer[2048], result[1024];

    if(verbose) {
      puts("sread");
    }
    if (sread(fd, cmd, 6) != 1) {
      return 1;
    }

    if (memcmp(cmd, "shift:", 6)) {
      cmd[6] = 0;
      fprintf(stderr, "invalid cmd '%s'\n", cmd);
      return 1;
    }

    unsigned int len;
    if (sread(fd, &len, 4) != 1) {
      fprintf(stderr, "reading length failed\n");
      return 1;
    }

    unsigned int nr_bytes = (len + 7) / 8;
    if (nr_bytes * 2 > sizeof(buffer)) {
      fprintf(stderr, "buffer size exceeded %d[%d]\n", nr_bytes, sizeof(buffer));
      return 1;
    }

    if (sread(fd, buffer, nr_bytes * 2) != 1) {
      fprintf(stderr, "reading data failed\n");
      return 1;
    }

    memset(result, 0, nr_bytes);

    if (verbose) {
      printf("#");
      for (i = 0; i < nr_bytes * 2; ++i)
        printf("%02x ", buffer[i]);
      printf("\n");
    }

    //
    // Only allow exiting if the state is rti and the IR
    // has the default value (IDCODE) by going through test_logic_reset.
    // As soon as going through capture_dr or capture_ir no exit is
    // allowed as this will change DR/IR.
    //
    seen_tlr = (seen_tlr || jtag_state == test_logic_reset) && (jtag_state != capture_dr) && (jtag_state != capture_ir);


    //
    // Due to a weird bug(??) xilinx impacts goes through another "capture_ir"/"capture_dr" cycle after
    // reading IR/DR which unfortunately sets IR to the read-out IR value.
    // Just ignore these transactions.
    //

    if ((jtag_state == exit1_ir && len == 5 && buffer[0] == 0x17) ||
        (jtag_state == exit1_dr && len == 4 && buffer[0] == 0x0b)) {
      if (verbose)
        printf("ignoring bogus jtag state movement in jtag_state %d\n", jtag_state);
    } else {
      for (i = 0; i < len; ++i) {
        //
        // Do the actual cycle.
        //

        int tms = !!(buffer[i / 8] & (1 << (i & 7)));
        //
        // Track the state.
        //
        jtag_state = jtag_step(jtag_state, tms);
      }
      if (io_scan(buffer, buffer + nr_bytes, result, len) < 0) {
        fprintf(stderr, "io_scan failed\n");
        exit(1);
      }
      if(verbose) {
        printTDO(result, len);
      }
    }

    if (send(fd, (const char*)result, nr_bytes, 0) != nr_bytes) {
      printf("Send failed with error: %d\n", WSAGetLastError());
      return 1;
    }

    if (verbose) {
      printf("jtag state %d\n", jtag_state);
    }
  } while (!(seen_tlr && jtag_state == run_test_idle));
  if(verbose) {
    puts("exit handle_data");
  }
  return 0;
}

int main(int argc, char **argv) {
  int ret = 0;
  int i;
  SOCKET s;
  int c;
  struct sockaddr_in address;
  unsigned short  port = DEFAULT_PORT;
  unsigned short jtagSpeed = DEFAULT_JTAG_SPEED;
  unsigned int coreId = 0;

  opterr = 0;

  while ((c = getopt(argc, argv, "vs:p:i:")) != -1)
    switch (c) {
      case 'v':
        verbose = 1;
        break;
      case 's':
        jtagSpeed = (unsigned short)strtoul(optarg, NULL, 0);
        if(verbose) {
          printf("Will use jtag speed %d kHz\n", jtagSpeed);
        }
        break;
      case 'p':
        port = (unsigned short)strtoul(optarg, NULL, 0);
        if(verbose) {
          printf("Will use port %d\n", port);
        }
        break;
      case 'i':
        coreId = strtoul(optarg, NULL, 0);
        if(verbose) {
          printf("Will search core id 0x%.8X\n", coreId);
        }
        break;
      case '?':
        fprintf(stderr, "usage: %s [-v] [-p port] [-s jtag_speed_in_kHz] [-i core_id]\n", *argv);
        return 1;
    }

  if (io_init(jtagSpeed, coreId)) {
    fprintf(stderr, "io_init failed\n");
    return 1;
  }

  WSADATA wsaData;

  int iResult;

  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
  if (iResult != 0) {
    printf("WSAStartup failed: %d\n", iResult);
    return 1;
  }

  s = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);

  if (s == INVALID_SOCKET) {
    printf("Error at socket(): %d\n", WSAGetLastError());
    WSACleanup();
    return 1;
  }

  i = 1;
  setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&i, sizeof i);

  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);
  address.sin_family = AF_INET;

  iResult = bind(s, (struct sockaddr *) &address, sizeof(address));

  if (iResult == SOCKET_ERROR) {
    printf("Bind failed with error: %d\n", WSAGetLastError());
    closesocket(s);
    WSACleanup();
    return 1;
  }

  if ( listen( s, SOMAXCONN ) == SOCKET_ERROR ) {
    printf( "Listen failed with error: %d\n", WSAGetLastError() );
    closesocket(s);
    WSACleanup();
    return 1;
  }

  fd_set conn;
  int maxfd = 0;

  FD_ZERO(&conn);
  FD_SET(s, &conn);

  maxfd = s;

  if (verbose) {
    puts("Started");
  }
  while (1) {
    fd_set read = conn, except = conn;
    SOCKET fd;

    //
    // Look for work to do.
    //

    if (select(maxfd + 1, &read, 0, &except, 0) == SOCKET_ERROR) {
      printf( "Select failed with error: %d\n", WSAGetLastError() );
      break;
    }

    for (fd = 0; fd <= maxfd; ++fd) {
      if (FD_ISSET(fd, &read)) {
        //
        // Readable listen socket? Accept connection.
        //

        if (fd == s) {
          SOCKET newfd;
          int nsize = sizeof(address);

          newfd = accept(s, (struct sockaddr*)&address, &nsize);
          if (verbose) {
            printf("connection accepted - fd %d\n", newfd);
          }
          if (newfd == INVALID_SOCKET) {
            printf("accept failed with error: %d\n", WSAGetLastError());
          } else {
            puts("setting TCP_NODELAY to 1");
            int flag = 1;
            int optResult = setsockopt(newfd,
                                       IPPROTO_TCP,
                                       TCP_NODELAY,
                                       (char *)&flag,
                                       sizeof(flag));
            if (optResult < 0) {
              perror("TCP_NODELAY error");
            }
            if (newfd > maxfd) {
              maxfd = newfd;
            }
            FD_SET(newfd, &conn);
          }
        }
          //
          // Otherwise, do work.
          //
        else {
          int r;
          if(verbose) {
            puts("start handle_data");
          }
          r = handle_data(fd);
          if(verbose) {
            puts("stop handle_data");
          }
          if (r) {
            //
            // Close connection when required.
            //

            if (verbose)
              printf("connection closed - fd %d\n", fd);
            closesocket(fd);
            FD_CLR(fd, &conn);
          }
        }
      }
        //
        // Abort connection?
        //
      else if (FD_ISSET(fd, &except)) {
        if (verbose) {
          printf("connection aborted - fd %d\n", fd);
        }
        closesocket(fd);
        FD_CLR(fd, &conn);
        if (fd == s)
          break;
      }
    }
  }
  closesocket(s);
  WSACleanup();
  //
  // Un-map IOs.
  //
  io_close();

  return 0;
}
