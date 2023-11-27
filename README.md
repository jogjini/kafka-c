# kafka-c

This program is  reads data from a file and sends it to the kafka topic of your choice. The consumer then reads all the data contained in the topic and stores it in another file.



# Install Kafka
## Install Kafka Servers FIRST !
You can install kafka servers from the apache kafka website : https://kafka.apache.org/downloads 
## Then install kafka C/C++ library 
### MacOS 
```bash
$ brew install librdkafka
```
### Debian-Ubuntu
```bash
$ apt install librdkafka-dev
```

# Run Kafka and create topic
## Run zookeeper and kafka servers
To be able to use a Kafka topic, you have to start the servers first. Go to the main folder of the Kafka Servers that you have installed in the first step and follow these steps : 
First run Zookeeper server : 
  ```bash
kafka_2.12-3.6.0 % ./bin/zookeeper-server-start.sh config/zookeeper.properties
```
Second Run Kafka server :   
```bash
kafka_2.12-3.6.0 % ./bin/kafka-server-start.sh config/server.properties
```
If you are having trouble with running these scripts, try running them in "sudo" mode.
To shut down the servers after you are finished, run command+c on the Kafka server first and then do the same for the zookeeper server.

## Create a new topic

  You can create a new topic on your cluster using the following command : <br>
  ```bash
  kafka_2.12-3.6.0 % bin/kafka-topics.sh --create --bootstrap-server localhost:9092 --replication-factor 1 --partitions 3 --topic topic-name
  ```


# Run the producer and consumer
## Compile files 
```bash
make all
```

## Run producer 
```bash
./producer localhost:9092 topic-name data_file_path
```
## Run consumer
```bash
./consumer localhost:9092 groupid topic-name
```
#BerlinMOD-kafka
The BerlinMOD folder contains a program that assembles and clips synthetic trip data generated by the MobilityDB-BerlinMOD. 
## Assemble_kafka.c
This script assembles the trips from the individual observations contained in the "trip_instants.csv" and sends the assembled trips to a kafka topic.
## Clip_kafka.c
This scripts reads the assembled trips data that were sent by the previous script from the same kafka topic and generate statics about the Brussels communes (or municipalities) traversed by the trips.


## Steps to take before running the program
To be able to execute the program, you will have to start the Kafka servers first (just like explained above). Then you will have to create a topic that will be able to accept much larger messages than it can by default. To be able to achieve this, you can create such a topic with the following command :
```bash
  kafka_2.12-3.6.0$ bin/kafka-topics.sh --create --bootstrap-server localhost:9092 --replication-factor 1 --partitions 3 --topic largeMessage --add-config max.message.bytes=10485880
```
The important parameter here is "max.message.byte", which specifies the maximum lenght of the messages passing through the topic.

## How to run the program
### Compile files 
```bash
berlinMOD-Kafka/make all
```
### Run producer
```bash
./berlinMOD-Kafka/assemble trip_instants.csv largeMessage
```
### Run consumer
```bash
./berlinMOD-Kafka/clip localhost:9092 group1 largeMessage
```

   


