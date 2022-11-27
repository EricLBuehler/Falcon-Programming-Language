string loadFile(string name){
    FILE* file=fopen(name.c_str(), "rb");
    if (file==NULL){
        fclose(file);
        cout<<"Cannot open file '"<<name<<"'.";
        exit(1);
    }
    fclose(file);

    fstream f(name, fstream::in );
    string s;
    getline( f, s, '\0');

    f.close();

    return s;
}