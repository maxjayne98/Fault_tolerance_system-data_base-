// #include <dirent.h>
// #include <iostream>
// #include <vector>
// #include <cstring>
// #include <string>
// #include <unistd.h>
// #include <sstream>
// #include <list>
// #include <sys/wait.h>
// #include <sys/stat.h>
// #include <fstream>

// using namespace std;

// #define PATH "./databases"
// #define QUIT_COMMAND "quit"
// #define FIFO_PATH "/tmp/os_ca2"
// #define WORKER_FILENAME "worker"
// #define PRESENTER_FILENAME "presenter"
// #define LAST_SENSOR_NUMBER 50
// #define LOAD_BALANCER_TAG "IAMLB"
#include"defines.hpp"


char* concat(const char* s1, const char* s2)
{
    char* result = (char*)malloc(strlen(s1) + strlen(s2) + 1); // +1 for the null-terminator
    // in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}
bool is_num(string s1) {
    for (size_t i = 0; i < s1.length(); ++i)
        if (s1[i] > '9' || s1[i] < '0')
            return false;
    return true;
}

void find_files(const char * path, vector<string> &files,int & counter){

    DIR *dir = opendir(path);

    struct dirent *entry = readdir(dir);

    while (entry != NULL)
    {
        // if (entry->d_type == DT_REG) //FOR FILES
        // if (entry->d_type == DT_DIR) //FOR DIR

        //subdirectory finded
        if (entry->d_type == DT_DIR){
            if (strcmp(entry -> d_name, ".") && strcmp(entry -> d_name, "..")) { // '.' and '..'

                //generate address for a subdirectory
                char* new_dir = concat("/", entry -> d_name);
                new_dir = concat(path, new_dir);
                //goto subdirectory
                find_files(new_dir,files,counter);
            }
        }

        //subfile finded
        if (entry->d_type == DT_REG && strcmp(path, ".")) {
            char* temp_dir = concat(path,"/");
            files.push_back(concat(temp_dir,entry -> d_name));
        }

        entry = readdir(dir);
    }

    closedir(dir);
    return ;
}


bool sensor_is_valid(string input){
    int inp;
    stringstream ss(input);
    ss >> inp;
    if ( inp >= LAST_SENSOR_NUMBER || inp < 0) {
        return false;
    }

    return true;
}

bool is_command_valid(string command){
        if(is_num(command) == 0){
            cout<<"--- Wrong command ---"<<endl<<"Try again or --- quit ---"<<endl<<endl;
            return false;
        }
        if (sensor_is_valid(command) == 0) {
             cout<<"--- This is not a valid sensor ---"<<endl<<"Try again or --- quit ---"<<endl<<endl;
             return false;
        }
    return true;
}

int main()
{
    string input;

    cout << ">>> Fault Tolerance System By Mohammad Mahdi Jahed 810195518 <<<" << endl << endl;

    unlink(FIFO_PATH);

    if (mkfifo(FIFO_PATH,0666) < 0) {
        cerr << "error in creating fifo." << endl;
        exit(1);
    }
    pid_t presenter_pid = fork();
    if (presenter_pid == 0) {
        char* argv[] = {NULL};
        execve(PRESENTER_FILENAME, argv, NULL);
    }
    
    

    while(getline(cin,input)){
        vector <pid_t> workers_pid;

        if (input == QUIT_COMMAND){
            cout << "--- BYE BYE! ---"<<endl;
            return 0;
        }

        if (is_command_valid(input) == 0) {
            continue;
        }
        
        vector<string> files;
        int count = 0;
        find_files(PATH,files,count);

        stringstream ss;

        //send numbre of sensor to the presenter
        ss << LOAD_BALANCER_TAG << endl << to_string(files.size()) << endl;
        ofstream pipe_stream (FIFO_PATH, ofstream::in | ofstream::out);
        if (pipe_stream.is_open()) {
            pipe_stream << ss.str();
            pipe_stream.close();
        }
        

        //crate child for start searching
        for(int i = 0; i < files.size(); i++){
                int fd[2];
            if (pipe(fd) != 0) {
                    cerr << "failed to create pipe." << endl;
                    exit(1);
            }

            pid_t worker_pid = fork();
            if (worker_pid == 0) {

                close(fd[1]);

                if (dup2(fd[0], STDIN_FILENO) == -1) {
                cerr << "failed to duplicate pipe fd to cin." <<endl;
                    exit(1);
                }
                
                char* argv[] = {NULL};
                execve(WORKER_FILENAME, argv, NULL);
            }
            else if(worker_pid > 0 ){

                workers_pid.push_back(worker_pid);
                close(fd[0]);

                stringstream sss;
                sss<<files[i]<<endl<<input<<endl;

                if (write(fd[1], sss.str().c_str(), sss.str().length()) < 0) {
                        cerr << "failed to write on unnamed pipe." << endl;
                        exit(1);
                    }
                close(fd[1]);
            }
        }

        for(int i = 0; i < workers_pid.size(); i++)
             waitpid(workers_pid[i], NULL, (int)NULL);

        files.clear();
    }

    waitpid(presenter_pid, NULL, (int)NULL);
    //nabayad bara peresenter to while ham sabr konim ? 
    return 0;
}