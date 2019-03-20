#ifndef _DEFINES_H
#define _DEFINES_H

#include <dirent.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sstream>
#include <list>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fstream>
#include <bits/stdc++.h> 

using namespace std;

#define PATH "./databases"
#define QUIT_COMMAND "quit"
#define FIFO_PATH "/tmp/os_ca2"
#define WORKER_FILENAME "worker"
#define PRESENTER_FILENAME "presenter"
#define LAST_SENSOR_NUMBER 50
#define LOAD_BALANCER_TAG "IAMLB"
#define WORKER_TAG "IAMWO"
#endif