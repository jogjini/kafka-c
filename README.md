# kafka-c

This program is  reads data from a file and sends it to the kafka topic of your choice. The consumer then reads all the data contained in the topic and stores it in another file.



# Install Kafka
You can install kafka from the apache kafka website : https://kafka.apache.org/downloads 
## Install kafka C/C++ library 
### MacOS 
$ brew install librdkafka
### Debian-Ubuntu
$ apt install librdkafka-dev

# Run Kafka and create topic
## Run zookeeper and kafka servers
<p>Go to the main folder of Kafka that you have installed and follow these steps : <br>
Run Zookeeper server : kafka_2.12-3.6.0 % ./bin/zookeeper-server-start.sh config/zookeeper.properties <br>
Run Kafka server :   kafka_2.12-3.6.0 % ./bin/kafka-server-start.sh config/server.properties <br>
You can edit zookeeper.properties and server.properties according to your needs </p>

## Create a new topic

  You can create a new topic on your cluster using the following command : <br>
  ```console
  kafka_2.12-3.6.0 % bin/kafka-topics.sh --create --bootstrap-server localhost:9092 --replication-factor 1 --partitions 3 --topic topic-name
  ```


# Run the producer and consumer
## Compile files 
make all

## Run producer 
./producer localhost:9092 topic-name data_file_path

## Run consumer
./consumer localhost:9092 groupid topic-name




