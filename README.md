# kafka-c
Example of use of kafka in c

# Install Kafka
You can install kafka from the apache kafka website : https://kafka.apache.org/downloads 
## Install kafka C/C++ library 
### MacOS 
$ brew install librdkafka
### Debian-Ubuntu
$ apt install librdkafka-dev

# Run Kafka 
Go to the main folder of Kafka that you have installed and follow these steps : 
Run Zookeeper server : kafka_2.12-3.6.0 % ./bin/zookeeper-server-start.sh config/zookeeper.properties
Run Kafka server : kafka_2.12-3.6.0 % ./bin/kafka-server-start.sh config/server.properties
You can edit zookeeper.properties and server.properties according to your needs

