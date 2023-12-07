FLAGS= -L/usr/include
LINKS = -lrdkafka -lmeos -L/usr/local/lib

expand_files = ais_expand_full.c ../initialize_kafka.c

expand : ais_expand_full.c ../initialize_kafka.h
	gcc ${FLAGS} ${expand_files} -o expand ${LINKS}

all : expand