FLAGS= -L/usr/include/librdkafka
LINKS = -lrdkafka

shared_files = initialize_kafka.c
consumer_files = consumer.c process_messages.c
producer_files = producer.c
consumer: consumer.c process_messages.h initialize_kafka.h
	gcc ${FLAGS} ${consumer_files} ${shared_files} -o consumer ${LINKS}

producer: producer.c initialize_kafka.h
	gcc ${FLAGS} ${producer_files} ${shared_files} -o producer ${LINKS}

all : consumer producer
