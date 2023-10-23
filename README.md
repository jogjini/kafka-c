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
Go to the main folder of Kafka that you have installed and follow these steps : <br>
Run Zookeeper server : kafka_2.12-3.6.0 % ./bin/zookeeper-server-start.sh config/zookeeper.properties <br>
Run Kafka server : kafka_2.12-3.6.0 % ./bin/kafka-server-start.sh config/server.properties <br>
You can edit zookeeper.properties and server.properties according to your needs <br>

