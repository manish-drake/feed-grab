#ifndef GRABBER_H
#define GRABBER_H

class Grabber{
public:
    Grabber(){

    }

    void error(const char *msg);
    void start(int argPortno);
    void save(char* buff, int forPort);
    ~Grabber();
};


#endif //SERVER_H
