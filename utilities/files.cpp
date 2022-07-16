string loadFile(string name){
    fstream f(name, fstream::in );
    string s;
    getline( f, s, '\0');

    f.close();

    return s;
}