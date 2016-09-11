#!/bin/bash

#hdfs_dir=dmlc

#${HADOOP_HOME}/bin/hadoop fs -rm -r -f $hdfs_dir/data
#${HADOOP_HOME}/bin/hadoop fs -mkdir $hdfs_dir/data
#${HADOOP_HOME}/bin/hadoop fs -put ../data/agaricus.txt.train $hdfs_dir/data
#${HADOOP_HOME}/bin/hadoop fs -put ../data/agaricus.txt.test $hdfs_dir/data

#cat <<< "
#train_data = \"hdfs://${hdfs_dir}/data/agaricus.txt.train\"
#val_data = \"hdfs://${hdfs_dir}/data/agaricus.txt.test\"
#max_data_pass = 3
#" >guide/demo_hdfs.conf

./repo/dmlc-core/tracker/dmlc_tracker/dmlc_yarn.py --jobname dmlc_wxs --vcores 1 -mem 512 -n 2 -s 1 train dmlc.linear.conf --log_dir=log --sync_timeout 500 -alsologtostderr -v 10 
