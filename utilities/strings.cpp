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

    return str;
}