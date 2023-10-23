FLAGS= -lrdkafka

consumer_files = consumer.c process_messages.c
producer_files = producer.c
consumer: consumer.c process_messages.h
	gcc ${FLAGS} ${consumer_files} -o consumer

producer: producer.c
	gcc ${FLAGS} $(producer_files) -o producer

all : consumer producer
