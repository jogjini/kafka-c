FLAGS= -L/usr/include
LINKS = -lrdkafka -lmeos -L/usr/local/lib

consumer_files = clip_kafka.c ../initialize_kafka.c
producer_files = assemble_kafka.c ../initialize_kafka.c
consumer: clip_kafka.c ../initialize_kafka.h
	gcc ${FLAGS} ${consumer_files} -o clip ${LINKS}

producer: assemble_kafka.c ../initialize_kafka.h
	gcc ${FLAGS} ${producer_files} -o assemble ${LINKS}

all : consumer producer