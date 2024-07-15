#include <iostream>
#include <thread>
#include <fstream>
#include <lsp/client.h>
using namespace nlohmann;

bool readFile(std::string& path, std::string* data) 
{
    std::ifstream ifStream(path, std::ios::binary | std::ios::in);
    if (ifStream.is_open()) {
        ifStream.seekg(0, std::ios::end);
        int len = ifStream.tellg();
        ifStream.seekg(0, std::ios::beg);
        char* buff = new char[len];
        ifStream.read(buff, len);
        data->append(buff);
        delete[]buff;
    }
    else {
        std::cout << "file open fail!!!" << std::endl;
        return false;
    }
    return true;
}

int main() {
    std::string file1_uri = "file:///D:/c_workstation/projects/Third_Fork_Projects/lsp-cpp/test/src/main.cpp";
    std::string file1_path = "D:/c_workstation/projects/Third_Fork_Projects/lsp-cpp/test/src/main.cpp";
    std::string root_uri = "file:///D:/c_workstation/projects/Third_Fork_Projects/lsp-cpp";
    //std::string file1_text;
    //std::string file2_text;
    ProcessLanguageClient client(R"(D:\c_workstation\soft_tool\clangd_17.0.3\bin\clangd.exe)");
    MapMessageHandler msgHandler;
    auto debugFunc = [](value& j) {
        //value method = j.at("method");
        std::cout << "========================================\n";
        std::cout << j << std::endl;
    };
    msgHandler.bindNotify(METHOD_PublishDiagnostics.c_str(), debugFunc);
    msgHandler.bindNotify(METHOD_DidOpen.c_str(), debugFunc);
    msgHandler.bindNotify(METHOD_DidClose.c_str(), debugFunc);

    std::thread thread([&] {
        client.loop(msgHandler);
    });

    int res;
    while (scanf("%d", &res)) {
        switch (res)
        {
        case 0: // 退出
        {
            client.Exit();
            thread.detach();
            return 0;
            break;
        }
        case 1: // 初始化
        {
            msgHandler.bindResponse(client.Initialize(string_ref(root_uri)), debugFunc);
            break;
        }
        case 2: // 打开文件
        {
            std::string text;
            readFile(file1_path, &text);
            client.DidOpen(file1_uri, text);
            break;
        }
        case 3: // 关闭文件
        {
            client.DidClose(string_ref(file1_uri));
            break;
        }
        case 4: // 获取词语列表
        {
            msgHandler.bindResponse(client.SemanticTokensALL(string_ref(file1_uri)), debugFunc);
            break;
        }
        case 5: // 悬停信息
        {
            Position pos{8, 13};
            msgHandler.bindResponse(client.Hover(string_ref(file1_uri), pos), debugFunc);
            break;
        }
        default:
            break;
        }
    }
    return 0;
}
