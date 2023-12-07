#include "initialize_kafka.h"



static volatile sig_atomic_t run = 1;

/**
 * @brief Signal termination of program
 */
static void stop (int sig)
{
  run = 0;
}


rd_kafka_t* initialize_consumer(const char* hostname, const char *grpid, const char* topicname)
{
  rd_kafka_t *rk;          /* Consumer instance handle */
  rd_kafka_conf_t *conf;   /* Temporary configuration object */
  rd_kafka_resp_err_t err; /* librdkafka API error code */
  char errstr[512];        /* librdkafka API error reporting buffer */
  const char *brokers = hostname;     /* Argument: broker list */
  const char *groupid = grpid;     /* Argument: Consumer group id */
  const char *topics = topicname;           /* Argument: list of topics to subscribe to */
  int topic_cnt = 1;           /* Number of topics to subscribe to */
  rd_kafka_topic_partition_list_t *subscription; /* Subscribed topics */

  /*
     * Create Kafka client configuration place-holder
     */
  conf = rd_kafka_conf_new();

  /* Set bootstrap broker(s) as a comma-separated list of
     * host or host:port (default port 9092).
     * librdkafka will use the bootstrap brokers to acquire the full
     * set of brokers from the cluster. */
  if (rd_kafka_conf_set(conf, "bootstrap.servers", brokers,
                        errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK)
  {
    fprintf(stderr, "%s\n", errstr);
    rd_kafka_conf_destroy(conf);
    return NULL;
  }

  /* Set the consumer group id.
   * All consumers sharing the same group id will join the same
   * group, and the subscribed topic' partitions will be assigned
  * according to the partition.assignment.strategy
   * (consumer config property) to the consumers in the group. */
  if (rd_kafka_conf_set(conf, "group.id", groupid,
                        errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK)
  {
    fprintf(stderr, "%s\n", errstr);
    rd_kafka_conf_destroy(conf);
    return NULL;
  }

  /* If there is no previously committed offset for a partition
   * the auto.offset.reset strategy will be used to decide where
   * in the partition to start fetching messages.
   * By setting this to earliest the consumer will read all messages
   * in the partition if there was no previously committed offset. */
  if (rd_kafka_conf_set(conf, "auto.offset.reset", "earliest",
                        errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK)
  {
    fprintf(stderr, "%s\n", errstr);
    rd_kafka_conf_destroy(conf);
    return NULL;
  }

  if (rd_kafka_conf_set(conf, "max.partition.fetch.bytes", "40000000", errstr,
                        sizeof(errstr)) != RD_KAFKA_CONF_OK)
  {
    fprintf(stderr, "%s\n", errstr);
    return NULL;
  }
  if (rd_kafka_conf_set(conf, "fetch.message.max.bytes", "40000000", errstr,
                        sizeof(errstr)) != RD_KAFKA_CONF_OK)
  {
    fprintf(stderr, "%s\n", errstr);
    return NULL;
  }

  /*
   * Create consumer instance.
   *
   * NOTE: rd_kafka_new() takes ownership of the conf object
   *       and the application must not reference it again after
   *       this call.
   */
  rk = rd_kafka_new(RD_KAFKA_CONSUMER, conf, errstr, sizeof(errstr));

  if (!rk)
  {
    fprintf(stderr,
            "%% Failed to create new consumer: %s\n", errstr);
    return NULL;
  }
  else
  {
    fprintf(stderr,
            "Creating consumer\n");
  }
  conf = NULL; /* Configuration object is now owned, and freed,
                      * by the rd_kafka_t instance. */


  /* Redirect all messages from per-partition queues to
   * the main queue so that messages can be consumed with one
   * call from all assigned partitions.
   *
   * The alternative is to poll the main queue (for events)
   * and each partition queue separately, which requires setting
   * up a rebalance callback and keeping track of the assignment:
   * but that is more complex and typically not recommended. */
  rd_kafka_poll_set_consumer(rk);


  /* Convert the list of topics to a format suitable for librdkafka */
  subscription = rd_kafka_topic_partition_list_new(topic_cnt);

  rd_kafka_topic_partition_list_add(subscription,
                                    topics,
          /* the partition is ignored
           * by subscribe() */
                                    RD_KAFKA_PARTITION_UA);

  /* Subscribe to the list of topics */
  err = rd_kafka_subscribe(rk, subscription);
  if (err)
  {
    fprintf(stderr,
            "%% Failed to subscribe to %d topics: %s\n",
            subscription->cnt, rd_kafka_err2str(err));
    rd_kafka_topic_partition_list_destroy(subscription);
    rd_kafka_destroy(rk);
    return NULL;
  }

  fprintf(stderr,
          "%% Subscribed to %d topic(s), "
          "waiting for rebalance and messages...\n",
          subscription->cnt);

  rd_kafka_topic_partition_list_destroy(subscription);

  return rk;
}

rd_kafka_t* initialize_producer(const char* hostname)
{
  rd_kafka_t *rk;        /* Producer instance handle */
  rd_kafka_conf_t *conf; /* Temporary configuration object */
  char errstr[512];      /* librdkafka API error reporting buffer */
  const char *brokers = hostname;   /* Argument: broker list */

  conf = rd_kafka_conf_new();

  /* Set bootstrap broker(s) as a comma-separated list of
   * host or host:port (default port 9092).
   * librdkafka will use the bootstrap brokers to acquire the full
   * set of brokers from the cluster. */
  if (rd_kafka_conf_set(conf, "bootstrap.servers", brokers, errstr,
                        sizeof(errstr)) != RD_KAFKA_CONF_OK)
  {
    fprintf(stderr, "%s\n", errstr);
    return NULL;
  }


  if (rd_kafka_conf_set(conf, "message.max.bytes", "20000000", errstr,
                        sizeof(errstr)) != RD_KAFKA_CONF_OK)
  {
    fprintf(stderr, "%s\n", errstr);
    return NULL;
  }

  if (rd_kafka_conf_set(conf, "queue.buffering.max.messages", "10000000", errstr,
                        sizeof(errstr)) != RD_KAFKA_CONF_OK)
  {
    fprintf(stderr, "%s\n", errstr);
    return NULL;
  }

  if (rd_kafka_conf_set(conf, "linger.ms", "500", errstr,
                        sizeof(errstr)) != RD_KAFKA_CONF_OK)
  {
    fprintf(stderr, "%s\n", errstr);
    return NULL;
  }

  /* Set the delivery report callback.
   * This callback will be called once per message to inform
   * the application if delivery succeeded or failed.
   * See dr_msg_cb() above.
   * The callback is only triggered from rd_kafka_poll() and
   * rd_kafka_flush(). */
  //rd_kafka_conf_set_dr_msg_cb(conf, dr_msg_cb);

  /*
   * Create producer instance.
   *
   * NOTE: rd_kafka_new() takes ownership of the conf object
   *       and the application must not reference it again after
   *       this call.
   */
  rk = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));


  if (!rk)
  {
    fprintf(stderr, "%% Failed to create new producer: %s\n",
            errstr);
    return NULL;
  }


  return rk;
}
