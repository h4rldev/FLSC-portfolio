#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

#include "../../include/cli.h"
#include "../../include/config.h"
#include "../../include/log.h"
#include "../../include/meta.h"

static void usage() {
  printf("USAGE: flscio [OPTIONS]\n"
         "Options:\n"
         "    -h, --help                     displays this message\n"
         "    -p, --port  [port number]      override port specified in "
         "config, is thrown away at program "
         "end\n"
         "    -s, --ssl                      toggles HTTPS/SSL (needs a cert "
         "and key in path)\n"
         "    -v, --verbose                  toggles verbose messaging "
         "(enables logs)\n\n"

         "© h4rl, 2024\n"
         "This program is licensed under %s\n"
         "Made with ❤ by h4rl\n",
         __LICENSE__);
  exit(0);
}

int parse_args(int *argc, char ***argv, Config *populated_args) {
  int local_argc = *argc;
  char **local_argv = *argv;

  int option_index = 0;
  int digital_optind = 0;
  unsigned int port_buf = 0;

  if (local_argc == 1)
    return 0;

  static struct option long_options[] = {
      {"help", no_argument, 0, 'h'},
      {"port", required_argument, 0, 'p'},
      // requires no arg due to being bool;
      {"ssl", no_argument, 0, 's'},
      // optional for security alerts about certificate
      {"verbose", no_argument, 0, 'v'},
      {0, 0, 0, 0}}; // end options_arr

  while (1) {
    int this_option_optind = optind ? optind : 1;

    char arg = getopt_long(local_argc, local_argv, ":hp:sv", long_options,
                           &option_index);

    if (arg == -1)
      break;

    switch (arg) {
    case 'h':
      usage();

    case 'p':
      port_buf = atoi(optarg);
      if (port_buf <= 0 || port_buf > 65535) {
        flscio_log(Error, "Unknown port \"%s\"", optarg);
        return -1;
      } else {
        populated_args->port = port_buf;
      }
      break;

    case 's':
      populated_args->ssl.ssl = !populated_args->ssl.ssl;
      break;

    case '?':
      flscio_log(Error, "invalid flag passed \"%s\"\n", local_argv[optind - 1]);
      flscio_log(Info, "running server on default configuration\n");
      return 0;

    default:
      printf("?? getopt returned character code 0%o ??\n", arg);
    }
  }

  return 0;
}
