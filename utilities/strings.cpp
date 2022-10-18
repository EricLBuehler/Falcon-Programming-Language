string replace_newlines(string str){
    //Replace the \n s
    size_t index = 0;
    while (true) {
        /* Locate the substring to replace. */
        index = str.find("\n", index);
        if (index == std::string::npos) break;

        /* Make the replacement. */
        str.replace(index, 1, "\\n");

        /* Advance index forward so the next iteration doesn't pick it up as well. */
        index += 2;
    }

    index = 0;
    while (true) {
        /* Locate the substring to replace. */
        index = str.find("\r", index);
        if (index == std::string::npos) break;

        /* Make the replacement. */
        str.replace(index, 1, "\\r");

        /* Advance index forward so the next iteration doesn't pick it up as well. */
        index += 2;
    }

    index = 0;
    while (true) {
        /* Locate the substring to replace. */
        index = str.find("\t", index);
        if (index == std::string::npos) break;

        /* Make the replacement. */
        str.replace(index, 1, "\\t");

        /* Advance index forward so the next iteration doesn't pick it up as well. */
        index += 2;
    }

    return str;
}

string remove_spaces(string word) {
    string newWord="";
    bool hitalpha=false;
    for (int i = 0; i < word.length(); i++) {
        if (word[i]!=' ' && hitalpha==false){
            hitalpha=true;
        }
        if (hitalpha){
            newWord += word[i];
        }
    }

    return newWord;
}
