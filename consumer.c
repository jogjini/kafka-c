/*
 * librdkafka - Apache Kafka C library
 *
 * Copyright (c) 2019, Magnus Edenhill
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/**
 * Simple high-level balanced Apache Kafka consumer
 * using the Kafka driver from librdkafka
 * (https://github.com/edenhill/librdkafka)
 */

#include <time.h>
#include "process_messages.h"
#include "initialize_kafka.h"





static volatile sig_atomic_t run = 1;

/**
 * @brief Signal termination of program
 */
static void stop (int sig)
{
    run = 0;
}



int main (int argc, char **argv)
{
  rd_kafka_t *rk;        /* Consumer instance handle */

  FILE * newfile = fopen("received_aisdata.csv", "w");

  /*
  * Argument validation
  */
  if (argc < 4)
  {
    fprintf(stderr,
            "%% Usage: "
            "%s <broker> <group.id> <topic1> <topic2>..\n",
            argv[0]);
    return 1;
  }



  /* Signal handler for clean shutdown */
  signal(SIGINT, stop);

  /* Subscribing to topics will trigger a group rebalance
  * which may take some time to finish, but there is no need
  * for the application to handle this idle period in a special way
  * since a rebalance may happen at any time.
  * Start polling for messages. */

  rk = initialize_consumer(argv[1], argv[2], argv[3]);

  if (rk == NULL)
  {
    printf(" KAFKA CLIENT HAS NOT CONFIGURED PROPERLY \n ");
    return 1;
  }
  int cont = 0;

  /* Real-Time MobilityDB */

  while (run )
  {
    cont++;
    rd_kafka_message_t *rkm;

    rkm = rd_kafka_consumer_poll(rk, 10); // ms


    if (!rkm)
    {
      continue;      /* Timeout: no message within 100ms,
      *  try again. This short timeout allows
      *  checking for `run` at frequent intervals.
      */
    }

    /* consumer_poll() will return either a proper message
    * or a consumer error (rkm->err is set). */
    if (rkm->err)
    {
      /* Consumer errors are generally to be considered
      * informational as the consumer will automatically
      * try to recover from all types of errors. */
      fprintf(stderr,
              "%% Consumer error: %s\n",
              rd_kafka_message_errstr(rkm));
      rd_kafka_message_destroy(rkm);
      continue;
    }

    if (!strcmp(rkm->payload, "end"))
    {
      stop(1);
      rd_kafka_message_destroy(rkm);

      continue;
    }

    fprintf(newfile,"%.*s", (int)rkm->len , (const char *)rkm->payload);

    rd_kafka_message_destroy(rkm);

  }


  /* Close the consumer: commit final offsets and leave the group. */

  fprintf(stderr, "%% Closing consumer\n");
  rd_kafka_consumer_close(rk);

  /* Destroy the consumer */
  rd_kafka_destroy(rk);
  fclose(newfile);
  return 0;
}
