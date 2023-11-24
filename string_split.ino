//#include <iostream>
//#include <string>
#include <string.h>
//#include <vector>

using namespace std;

int main()
{
    // s_str é a string com os dados a serem separados.
    String s_str = "11.00-22.00-33.00-44.00-55-"; // A STRING TEM QUE TERMINAR COM UM HÍFEN
    // O último valor tem que ter o mesmo formato: 00.00
    Serial.print("s_str incial: ");
    Serial.println(s_str);

    Serial.print("s_str size: ");
    Serial.println(s_str.length());

    char value[] = "00.00";

    for(int i = 0; i < s_str.length();){
        int pos = s_str.indexOf("-", i);
        if(pos == -1){
            break;
        }
        Serial.print("i: ");
        Serial.println(i);
        Serial.print(" pos: ");
        Serial.println(pos);
        //cout << "i: " << i << " pos: " << pos << endl;
        
        /*strcpy(value, (s_str.substr(i, pos)).c_str());
        cout << "value: " << value << endl;
        i = pos+1;
        memset(value, 0, sizeof(value));
        cout << "value limpo: " << value << endl;*/

        sprintf(value, "%.5s", (s_str.substring(i, pos)).c_str());
        Serial.print("value sprintf: ");
        Serial.println(value);
        //cout << "value sprintf: " << value << endl;
        i = pos+1;

        /*char test[5];
        std::string str( "Hello, world" );

        str.copy(test, 5);

        std::cout.write(test, 5);
        std::cout.put('\n');*/

    }
}
