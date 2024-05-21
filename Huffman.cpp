#include<bits/stdc++.h>
using namespace std;


// Hàm chuyển số thập phân sang số nhị phân 8 bit
string dec_to_byte(int n) {
    string s = "";
    while(n > 0) {
        if(n % 2 == 0) 
            s = '0' + s;
        else s = '1' + s;
        n /= 2;
    }
    while(s.size() < 8) {
        s = '0' + s;
    }
    return s;
}

//Hàm chuyển số nhị phân 8 bit sang số thập phân
int byte_to_dec(string s) {
    int n = 0;
    for(int i = 0; i < s.size(); i++) {
        n += pow(2, 8-i-1)*int(s[i]-48);
    }
    return n;
}

// định nghĩa 1 nút của cây Huffman
struct Node {
    char data; // kí tự
    int freq; // tần suất xuất hiện
    Node *left, *right; // con trỏ cho cái nút bên trái, phải
};

//Tạo 1 nút mới cho cây
Node* getNode(char c, int freq, Node* left, Node* right) {
    Node* node  = new Node();

    // Khởi tạo node mới
    node->data = c;
    node->freq = freq;
    node->left = left;
    node->right = right;

    return node;
}

//So sánh các nút dựa trên tần suất xuất hiện để sắp xếp
struct comp
{
	bool operator()(Node* l, Node* r)
	{
		return l->freq > r->freq;
	}
};

//Hàm để duyệt cây Huffman để tìm mã code cho từng kí tự đã nén
void encode(Node* root, string s, map<char, string> &HuffmanCode) {

    //Nếu nút mang giá trị null thì return
    if(root == nullptr)
        return;
    
    //nếu nút không có nút con trái và phải nghĩa là nút lá 
    //thực hiện lưu mã code của kí tự nút lá đó
    if(root->left == nullptr && root->right == nullptr) {
        HuffmanCode[root->data] = s;
    }

    //nếu không phải nút lá ta duyệt 2 nút con
    encode(root->left, s + "0", HuffmanCode);
    encode(root->right, s + "1", HuffmanCode);
}

//Hàm giải mã chuỗi được mã hóa
void decode(Node* root, int &i, string s, ofstream& outputFile) {

     //Nếu nút mang giá trị null thì return
    if(root == nullptr)
        return;
    
    //nếu là nút là thì in ra kí tự đó
    if(root->left == nullptr && root->right == nullptr) {
        outputFile << root->data;
        return;
    }

    //cộng chỉ số của chuỗi mã hóa đang xét
    i++;

    if(s[i] == '0') // nếu tại vị trí chuỗi mã hóa đang xét là 0 thì thực hiện đệ quy sang nút con bên trái để xét tiếp
        decode(root->left, i, s, outputFile);
    else decode(root->right, i, s, outputFile); // ngược lại xét nhánh con bên phải
}

//Hàm xây dựng cây Huffman
Node* BuildHuffmanTree(string text) {
    
    priority_queue<Node*, vector<Node*>, comp> p;
    map<char, int> freqMap;
    
    //Đếm tần suất xuất hiện của mỗi kí tự
    for(auto x : text) {
        freqMap[x]++;
    }

    //Với mỗi kí tự xuất hiện tạo thành 1 nút và thêm vào priority_queue
    for(auto x : freqMap) {
        Node* node = getNode(x.first, x.second, nullptr, nullptr);
        p.push(node);
    }
    
    //Tạo cây Huffman bắt cách duyệt 2 nút có tần số nhỏ nhất 
    //và tạo nút cha của 2 nút có tần số bằng tổng tần số 2 nút con
    while(p.size() > 1) {
        //Xóa 2 nút có tần số xuất hiện thấp nhất
        Node* l = p.top(); 
        p.pop();
        Node* r = p.top();
        p.pop(); 

        //Tạo nút cha cho 2 nút con đó
        //Sau đó thêm nút cho vào priority_queue
        int sum = l->freq + r->freq;
        Node* node_par = getNode('\0', sum, l, r); //Tạo nút cha là null 
        p.push(node_par);
    }

    Node* root = p.top(); //nút gốc của cây
    return root;
}

// Mã hóa cấu trúc của cây Huffman và ghi vào phần header của tệp nén
void encodeTree(Node* root, ofstream& file) {
    if (!root) return;

    if (!root->left && !root->right) {
        // Nếu đây là một nút lá, ghi '1' để chỉ định nút lá
        file.put('1');
        // Ghi ký tự của nút lá vào tệp
        file.put(root->data);
    } else {
        // Nếu đây là một nút không phải lá, ghi '0' để chỉ định nút không phải lá
        file.put('0');
        // Tiếp tục ghi cấu trúc cây từ cây con bên trái và cây con bên phải
        encodeTree(root->left, file);
        encodeTree(root->right, file);
    }
}

//Hàm nén file
void encodeFile(string& input, string& output) {

    map<char, string> HuffmanCode;

    ifstream inputFile(input);
    ofstream outputFile(output);
    string s;
    string t;
    while(getline(inputFile, t)) {
        s += t;
        s += '\n';
    }
    s.erase(s.size()-1);
    Node* root = BuildHuffmanTree(s);
    encode(root, "", HuffmanCode);
    
    encodeTree(root, outputFile);
    outputFile << "!" << endl;

    string a = "";
    for(auto x : s) {
        a += HuffmanCode[x];
    }
    int p = a.size()%8;
    int q = 8-p;
    while(q > 0) {
        a = a + '0';
        q--;
    }
    for(int i = 0; i < a.size(); i += 8) { 
        string temp = a.substr(i, 8);
        outputFile << char(byte_to_dec(temp));
    }
    outputFile << char(8-p);

}

//Hàm tạo lại cây huffman khi đọc header file nén
Node* loadHuffmanTree(ifstream& inputFile) {
    char bit;
    inputFile.get(bit);
    if(bit == '!') return NULL;
    if (bit == '1') {
        char ch;
        inputFile.get(ch);
        return getNode(ch, 0, NULL, NULL);
    }

    Node* left = loadHuffmanTree(inputFile);
    Node* right = loadHuffmanTree(inputFile);

    Node* node = getNode('0', 0, NULL, NULL);
    node->left = left;
    node->right = right;
    
    return node;
}

//Hàm giải nén File
void decodeFile(string input, string output) {

    ifstream inputFile(input);
    ofstream outputFile(output);

    
    //ifstream inputFile;
    //ofstream outputFile(output);
    //outputFile.open(output, ios::out);
    
    Node* root =  loadHuffmanTree(inputFile);
    string s;
    string t;

    getline(inputFile, t);
    while(getline(inputFile, t)) {
        s += t + '\n';
    }
    

    s.erase(s.size()-1);
    string res = "";
    for(int i = 0; i < s.size()-1; i++) {
        unsigned char temp = s[i];
        int e = int(temp);
        res += dec_to_byte(e);
    }
    unsigned char temp = s[s.size()-1];
    int o = int(temp);
    while(o > 0) {
        res.erase(res.size()-1);
        o--;
    }
    int i = -1;
    while(i < (int)res.size()-2) {
        decode(root, i, res, outputFile);
    }
    
    //inputFile.close();
    //outputFile.close();
    
}
int main() {

    bool check = true;
    while(check) {

        int n;
        cout << "Chon chuc nang: " << endl;
        cout << "1. Nen File" << endl << "2. Giai Nen File" << endl;
        cin >> n;
        if(n == 1) {
            string s, t;
            cout << "Nhap Ten File Can Nen: ";
            cin >> s;
            cout << "Dat Ten File Nen: ";
            cin >> t;
            encodeFile(s, t);
            cout << "Success..." << endl;
        }
        else {
            if(n == 2) {
                string s, t;
                cout << "Nhap Ten File Can Giai Nen: ";
                cin >> s;
                cout << "Dat Ten File Giai Nen: ";
                cin >> t;
                decodeFile(s, t);
                cout << "Success..." << endl;
            }
            else cout << "Error!" << endl;
        }

        char kt;
        cout << "Tiep tuc? (y/n)" << endl;
        cin >> kt;
        if(kt == 'Y' || kt == 'y') check = true;
        else check = false;
    }
    return 0;
}

