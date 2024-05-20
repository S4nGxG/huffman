#include<bits/stdc++.h>
using namespace std;

// định nghĩa 1 nút của cây Huffman
struct Node {
    char data; // kí tự
    int freq; // tần suất xuất hiện
    Node *left, *right; // con trỏ cho cái nút bên trái, phải
};

//Tạo 1 nút mới cho cây
Node* getNode(int freq, char c, Node* left, Node* right) {
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
        //if(l->freq == r->freq) return l->data > r->data;
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
void decode(Node* root, int &i, string s) {

     //Nếu nút mang giá trị null thì return
    if(root == nullptr)
        return;
    
    //nếu là nút là thì in ra kí tự đó
    if(root->left == nullptr && root->right == nullptr) {
        cout << root->data;
        return;
    }

    //cộng chỉ số của chuỗi mã hóa đang xét
    i++;

    if(s[i] == '0') // nếu tại vị trí chuỗi mã hóa đang xét là 0 thì thực hiện đệ quy sang nút con bên trái để xét tiếp
        decode(root->left, i, s);
    else decode(root->right, i, s); // ngược lại xét nhánh con bên phải
}

//Hàm xây dựng cây Huffman
void BuildHuffmanTree(string text) {

    priority_queue<Node*, vector<Node*>, comp> p;
    map<char, int> freqMap;
    
    //Đếm tần suất xuất hiện của mỗi kí tự
    for(auto x : text) {
        freqMap[x]++;
    }

    //Với mỗi kí tự xuất hiện tạo thành 1 nút và thêm vào priority_queue
    for(auto x : freqMap) {
        Node* node = getNode(x.second, x.first, nullptr, nullptr);
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
        cout << l->data << " " << r->data << endl;
        //Tạo nút cha cho 2 nút con đó
        //Sau đó thêm nút cho vào priority_queue
        int sum = l->freq + r->freq;
        Node* node_par = getNode(sum, '\0', l, r); //Tạo nút cha là null 
        p.push(node_par);
    }

    Node* root = p.top(); //nút gốc của cây

    //Tạo một map để lưu trữ mã code của kí tự sau khi nén
    map<char, string> HuffmanCode;

    //duyệt qua cây Huffman để tìm mã code của từng kí tự
    encode(root, "", HuffmanCode);

    //in ra mã code đã mã hóa của từng kí tự
    cout << "Huffman Code: " << endl;
    for(auto x : HuffmanCode)
        cout << x.first << ": " << x.second << endl;
    
    cout << endl << "String: " << text << endl;
    cout << "Size befor encode: " << text.size()*8 << endl;

    //nối các mã code của từng kí tự lại để thành chuỗi đã nén của dữ liệu
    string s = "";
    for(auto x : text)
        s += HuffmanCode[x];
    
    cout << endl << "Encode: " << endl << s << endl;
    cout << "Size: " << s.size() << endl;

    //duyệt qua cây Huffman và giải mã chuỗi đã mã hóa
    int i = -1;
    cout << endl << "Decode: " << endl;
    while(i < (int)s.size()-2) {
        decode(root, i, s);
    }
}

int main() {
    string s = "ABBCCCDDDDEEEEE";
    BuildHuffmanTree(s);
    return 0;
}

