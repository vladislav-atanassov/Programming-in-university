#ifndef APPLICATION_H
#define APPLICATION_H

class Application
{
public:
    void setup();
    void loop();

private:
    unsigned long lastPrintMs_ = 0;
};

#endif // APPLICATION_H
