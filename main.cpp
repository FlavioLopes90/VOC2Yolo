#include <iostream>
#include <fstream>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <boost/algorithm/string/find.hpp>

using namespace std;



#define path_images "/newdataset/pictures/"            // ------------- Dataset images. The *.txt annoations file will be stored in this folder
#define path_labels_voc "newdataset/labels/"           // ------------- Annotations VOC files *.xml
#define objnames_voc "newdataset/labelmap.pbtxt"       // ------------- Files with labels with VOC format
#define objnames "/newdataset/obj.names"               // ------------- Output File with labels with YOLO format



int readfile(string file_name, char *p1, std::vector<string> label_map);
int create_labelmap(string file_name);
std::vector<string> define_labelmap();

int main()
{

    std::vector<string> label_map;
    DIR *d;
    char *p1,*p2;
    int ret, n_files = 0;
    struct dirent *dir;

    create_labelmap(objnames_voc);

    label_map=define_labelmap();
    cout << "label map: " << endl;
    for(int i = 0; i < label_map.size(); i++) {
        cout << label_map[i] << " " << i << endl;
    }

    d = opendir(path_labels_voc);
    int kk=-2;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {

            kk++;
            cout << kk << endl;
            cout << "while " << endl;
            p1=strtok(dir->d_name,".");
            p2=strtok(NULL,".");

            printf("%s \n",p1);
            printf("%s \n",p2);

            if(p2!=NULL)
            {
                ret=strcmp(p2,"xml");
                if(ret==0)
                {
                    n_files=n_files+1;
                    //printf("%s \n",p1);
                    string file_name_final = path_labels_voc + (string)strcat(p1,".xml");
                    cout << file_name_final << endl;

                    int ret_rf=readfile(file_name_final, p1, label_map);
                    cout << ret_rf << endl;


                }
            }


        }
        closedir(d);
    }
    cout << "N FILES -> " << n_files << endl;
    return 0;

}

int readfile(string file_name, char *p1, std::vector<string> label_map)
{

    /*Array in the heap*/
    char *pString=new char[sizeof(p1)/sizeof(char)];
    strcpy(pString,p1);

    string sString=string(pString);
    free(pString);

    string outfile_name = path_images + sString.substr(0,sString.size()-4) + ".txt";
    cout << outfile_name << endl;

    std::ofstream outfile (outfile_name);

    ifstream myReadFile;
    myReadFile.open(file_name);

    int pos_f = 0, pos_i = 0;
    int x_min, x_max, y_min, y_max;
    string str_x_min, str_x_max, str_y_min, str_y_max;

    float dw ;
    float dh ;

    for( std::string line; getline( myReadFile, line ); )
    {

        cout << line << endl;
        if (line.substr(1,6) == "<size>")
        {
            getline( myReadFile, line );
            pos_f = 0; pos_i= 0;
            // Find delimiter '
            for(size_t pos=line.find("width",0); pos!=std::string::npos; pos=line.find("width",pos+1))
            {
                if(pos_i > 0)
                    pos_f=pos;
                else
                    pos_i = pos;
            }

            string str_dw = line.substr(pos_i+6,pos_f-pos_i-8);
            cout << str_dw << endl;
            dw = 1.0 / atoi(str_dw.c_str());

            getline( myReadFile, line );
            pos_f = 0; pos_i= 0;
            // Find delimiter '
            for(size_t pos=line.find("height",0); pos!=std::string::npos; pos=line.find("height",pos+1))
            {
                if(pos_i > 0)
                    pos_f=pos;
                else
                    pos_i = pos;
            }
            string str_dh = line.substr(pos_i+7,pos_f-pos_i-9);
            cout << str_dh << endl;
            dh = 1.0 / atoi(str_dh.c_str());
            getline( myReadFile, line );

        }

        if (line.substr(1,8) == "<object>")
        {
            getline( myReadFile, line );
            if (line.substr(2,6) == "<name>")
            {
                pos_f = 0; pos_i= 0;
                // Find delimiter '
                for(size_t pos=line.find("name",0); pos!=std::string::npos; pos=line.find("name",pos+1))
                {
                    if(pos_i > 0)
                        pos_f=pos;
                    else
                        pos_i = pos;
                }
                cout << line.substr(pos_i+5,pos_f-pos_i-7) << endl;

                for(int i = 0; i < label_map.size(); i++)
                {
                    if ( label_map[i] == line.substr(pos_i+5,pos_f-pos_i-7) )
                    {
                        outfile << i;
                        i=9999;
                    }
                }
                for(int i=0; i<5;i++)
                    getline( myReadFile, line );

                pos_f = 0; pos_i= 0;
                for(size_t pos=line.find("xmin",0); pos!=std::string::npos; pos=line.find("xmin",pos+1))
                {
                    if(pos_i > 0)
                        pos_f=pos;
                    else
                        pos_i = pos;
                }
                str_x_min = line.substr(pos_i+5,pos_f-pos_i-7);

                getline( myReadFile, line );
                pos_f = 0; pos_i= 0;
                for(size_t pos=line.find("ymin",0); pos!=std::string::npos; pos=line.find("ymin",pos+1))
                {
                    if(pos_i > 0)
                        pos_f=pos;
                    else
                        pos_i = pos;
                }
                str_y_min = line.substr(pos_i+5,pos_f-pos_i-7);

                getline( myReadFile, line );
                pos_f = 0; pos_i= 0;
                for(size_t pos=line.find("xmax",0); pos!=std::string::npos; pos=line.find("xmax",pos+1))
                {
                    if(pos_i > 0)
                        pos_f=pos;
                    else
                        pos_i = pos;
                }
                str_x_max = line.substr(pos_i+5,pos_f-pos_i-7);

                getline( myReadFile, line );
                pos_f = 0; pos_i= 0;
                for(size_t pos=line.find("ymax",0); pos!=std::string::npos; pos=line.find("ymax",pos+1))
                {
                    if(pos_i > 0)
                        pos_f=pos;
                    else
                        pos_i = pos;
                }
                str_y_max = line.substr(pos_i+5,pos_f-pos_i-7);

                x_min = atoi(str_x_min.c_str()) ;
                y_min = atoi(str_y_min.c_str()) ;
                x_max = atoi(str_x_max.c_str()) ;
                y_max = atoi(str_y_max.c_str()) ;

                outfile << " " << ((x_max+x_min)/2.0)*dw;
                outfile << " " << ((y_max+y_min)/2.0)*dh;
                outfile << " " << ((x_max-x_min))*dw;
                outfile << " " << ((y_max-y_min))*dh;


            }
            outfile << endl;
        }

        if (line.substr(0,13) == "</annotation>")
        {
            cout << "last line" << endl;
            //outfile.close();
        }
         line.clear();
    }

    //outfile.close();
    cout << "close file ok" << endl;
    return 0;
}

int create_labelmap(string file_name)
{

    string outfile_name = objnames;
    //string outfile_name = labels_voc.substr(0, labels_voc.size()-9) + "obj.names";
    //cout << outfile_name << endl;

    std::ofstream outfile (outfile_name, std::ofstream::out);

    ifstream myReadFile;
    myReadFile.open(file_name);

    for( std::string line; getline( myReadFile, line ); )
    {
        if (line.substr(0,6) == "item {")
        {
            getline( myReadFile, line );
            getline( myReadFile, line );

            int pos_f, pos_i =8;
            if (line.substr(1,6) == "name: ")
            {
                // Find delimiter '
                for(size_t pos=line.find("'",0); pos!=std::string::npos; pos=line.find("'",pos+1))
                    pos_f = pos-pos_i;

                cout << line.substr(pos_i,pos_f) << endl;
                outfile << line.substr(pos_i,pos_f);
                cout << outfile_name << endl;
                outfile << endl;
            }
        }
    }
    outfile.close();
}


std::vector<std::string> define_labelmap()
{

    ifstream myReadFile;
    myReadFile.open(objnames);
    std::vector<std::string> strVec;

    for( std::string line; getline( myReadFile, line ); )
        strVec.push_back(line);

    return strVec;
}
