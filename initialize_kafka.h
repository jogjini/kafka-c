#include <librdkafka/rdkafka.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>


static void stop(int sig);

rd_kafka_t* initialize_consumer(const char* hostname, const char *grpid, const char* topicname);

rd_kafka_t* initialize_producer(const char* hostname);