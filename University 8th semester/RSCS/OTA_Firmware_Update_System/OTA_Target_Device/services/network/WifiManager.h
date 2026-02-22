#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

class WifiManager
{
public:
    bool connect();
    bool isConnected() const;
    void ensureConnected();
};

#endif // WIFI_MANAGER_H
