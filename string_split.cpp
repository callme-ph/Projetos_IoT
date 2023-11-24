/***************************

- Função atof
https://www.ibm.com/docs/en/i/7.1?topic=functions-atof-convert-character-string-float
- Função sprintf
https://stackoverflow.com/questions/2889421/how-to-copy-a-string-into-a-char-array-in-c-without-going-over-the-buffer 

****************************/


#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <iterator>
#include <stdlib.h>

using namespace std;

int main()
{
    vector<float*> strings;

    // s_str é a string com os dados a serem separados.
    string s_str = "11.00-22.00-33.00-44.00-55-"; // A STRING TEM QUE TERMINAR COM UM HÍFEN
    // O último valor tem que ter o mesmo formato: 00.00
    cout << "s_str incial: " << s_str << endl;
    cout << "s_str size: " << s_str.length() << endl;

    char value[] = "00.00";

    for(int i = 0; i < s_str.length();){
        int pos = s_str.find("-", i);
        if(pos == -1){
            break;
        }
        cout << "i: " << i << " pos: " << pos << endl;

        /*strcpy(value, (s_str.substr(i, pos)).c_str());
        cout << "value: " << value << endl;
        i = pos+1;
        memset(value, 0, sizeof(value));
        cout << "value limpo: " << value << endl;*/

        sprintf(value, "%.5s", (s_str.substr(i, pos)).c_str());
        cout << "value sprintf: " << value << endl;
        i = pos+1;

        float result = atof(value);
        cout << "result: " << result << endl;;

        /*strings.push_back(result);
        copy(strings.begin(),strings.end(),ostream_iterator<string>(cout," "));
        cout << endl;*/

        /*char test[5];
        std::string str( "Hello, world" );

        str.copy(test, 5);

        std::cout.write(test, 5);
        std::cout.put('\n');*/

    }
}
