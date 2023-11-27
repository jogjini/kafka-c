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
To be able to use a Kafka topic with a server, you have to start the servers first. Go to the main folder of the Kafka Servers that you have installed in the first step and follow these steps : 
First run Zookeeper server : 
  ```bash
kafka_2.12-3.6.0 % ./bin/zookeeper-server-start.sh config/zookeeper.properties 
```
Second Run Kafka server :   
```bash
kafka_2.12-3.6.0 % ./bin/kafka-server-start.sh config/server.properties 
```
If you are having trouble with running these scripts, try running them in "sudo" mode.

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




