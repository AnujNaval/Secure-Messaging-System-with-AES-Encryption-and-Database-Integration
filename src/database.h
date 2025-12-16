#ifndef DATABASE_H
#define DATABASE_H

#include <string>

void setup_database();
void store_message(const std::string& username, const std::string& message,
                   const unsigned char* iv);

#endif // DATABASE_H
