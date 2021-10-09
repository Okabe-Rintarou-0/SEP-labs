**回答问题**：

1. 请解释在`Editor::dispatchCmd`中下述代码的逻辑，尤其注意对stringstream的使用。

    int start, end;
    char comma, type = ' ';
    stringstream ss(cmd);
    ss >> start;
    if (ss.eof()) {
        cmdNull(start);
        return;
    }
    ss >> comma >> end >> type;
    if (ss.good()) {
        if (type == 'n') {
            cmdNumber(start, end);
            return;
        } else if (type == 'd') {
            cmdDelete(start, end);
            return;
        }
    }

该段代码通过将输入的命令行放入stringstream对象ss中，便于读取命令行字符串中的数字、字符等。

ss>>start读取一串数字，如果该数字读完后正好遇到eof，那么证明命令行只包含一个数字，那么就是nullcmd。

接着读取逗号，end和type，分别是一个字符，数字和一个字符，ss.good判断是否读入正常（符合读取类型），如果正常，那就是?,?n指令或者?.?d指令，在两者中通过type进行区分即可。

