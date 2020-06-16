//10720116 鄭宇傑 10720138 陳尚宏 請使用此GitHub版本機測

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <istream>
#include <sstream>
#include <vector>
#include <iterator>
#include <bits/stdc++.h>
#include <algorithm>


using namespace std;
#define MAX_LEN 10 // array size of student id and name
#define PAGE 25 // amount of display on screen
#define NONE -1 // error flag


typedef struct sP {
    char sid1[MAX_LEN]; // 1st sid: sender
    char sid2[MAX_LEN]; // 2nd sid: receiver
    float wgt; // pair weight
} studentPair;

typedef struct aLN { // node of adjacency lists
    string sid2; // receiver
    float weight; // pair weight
    //struct aLN *next; // pointer to the next node
} adjListNode;

typedef struct aL {// adjacency list
    string sid1; // sender
    vector<adjListNode> listnode; //nodes of adjacency lists
    //adjListNode *head; // pointer to the first node of a list
    float inf = 0; // influence value
    bool visited = false ;
} adjList;



typedef struct aLcnt {// adjacency list
    vector<string> sid; // Connected Component
    bool finished = false;
} adjListcnt;




/*
typedef struct sT {
    char sid[MAXLEN];
    char sname[MAXLEN];
    unsigned char score[COLUME];
    float mean;
} studentType;
*/

class Graph {
private:
public:
    vector<studentPair> aList;
    vector<adjList> adjL; // the adjacency lists
    vector<adjList> infL; // a list of all influence values
    vector<adjListcnt> cntCpm; // Connected Component
    int nodecounts = 0 ;
    int outputnodecount = 0 ;
    float inputweight = 0;

    void readBinary(string);
    void createASJlist();
    void createASJlist_wgt();
    void saveFileASJlist(string);
    void saveFileASJlist_infL(string);
    void saveDFS(string);


    void DFS();
    void dijkstra(string);
    void savedijkstra(string, string );

    bool fileexisted = false;

};

bool compareBy_adjList_sid1(const adjList &a, const adjList &b) {

    return atoi(a.sid1.c_str())<atoi(b.sid1.c_str());


}

bool compareBy_adjListNode_sid2(const adjListNode &a, const adjListNode &b) {

    return atoi(a.sid2.c_str())<atoi(b.sid2.c_str());
    /*
        if (atoi(a.sid2.c_str())<atoi(b.sid2.c_str())) {
                cout<<endl<<"TRUE"<<endl;
            return true;
        } else {
            cout<<endl<<"FASLE"<<endl;
            return false;
        }
    */

}


bool compareBy_adjList_inf(const adjList &a, const adjList &b) {

    return a.inf<b.inf;


}

bool compareBy_string(const string &a, const string &b) {

    return atoi(a.c_str())<atoi(b.c_str());


}


bool compareBy_adjListcnt_size(const adjListcnt &a, const adjListcnt &b) {

    return a.sid.size()>b.sid.size();


}

struct FindByName {
    const string name;
    FindByName(const string& name) : name(name) {}
    bool operator()(const adjList& j) const {
        return j.sid1 == name;
    }
};

struct FindByName_sid2 {
    const string name;
    FindByName_sid2(const string& name) : name(name) {}
    bool operator()(const adjListNode& j) const {
        return j.sid2 == name;
    }
};



void Graph::readBinary(string filename) {

    fstream binfile;
    studentPair oneSt;
    int stNo=0;
    //filename="input"+filename+".bin";
    cout<<filename<<endl;
    binfile.open(filename.c_str(),fstream::in|fstream::binary);

    if(binfile.is_open()) {
        fileexisted = true;
        binfile.seekg(0,binfile.end);
        stNo=binfile.tellg()/sizeof(oneSt);
        nodecounts=stNo;
        cout<<"nodecounts= "<<nodecounts<<endl;

        binfile.seekg(0,binfile.beg);

        for(int i=0; i<stNo; i++) {
            studentPair temp;
            binfile.read((char*)&oneSt,sizeof(oneSt));
            //cout<<"["<<i+1<<"]"<<oneSt.sid1<<","<<oneSt.sid2<<" ,"<<oneSt.wgt<<endl;

            aList.push_back(oneSt);
        }
    } else {
        cout<<"File not exist."<<endl;
        fileexisted = false;
        return;


    }
    /*
    vector<studentPair>::iterator iter = aList.begin();
    for(iter; iter!= aList.end(); iter++) {
        binfile.read((char*)&oneSt,sizeof(oneSt));
        cout<< iter->sid1 << "\t"
            << iter->sid2 << endl
            << iter-> wgt << endl;
    }
    */

    binfile.close();

}




void Graph::createASJlist() {

    vector<studentPair>::iterator iter = aList.begin();
    for(iter; iter!= aList.end(); iter++) {

        /*cout<< iter->sid1 << "\t"
            << iter->sid2 << "\t"
            << iter-> wgt << endl;*/

        string inputsid1(iter->sid1);
        string inputsid2(iter->sid2);
        float inputwgt = iter-> wgt;

        //create graph by sid1

        vector<adjList>::iterator it = find_if(adjL.begin(), adjL.end(), FindByName(inputsid1));

        if(it == adjL.end()) { //graph not exist
            adjList tempadjList;
            tempadjList.sid1=inputsid1;

            adjL.push_back(tempadjList); //create new graph

            it = find_if(adjL.begin(), adjL.end(), FindByName(inputsid1));

            //create new node
            adjListNode tempadjListNode;
            tempadjListNode.sid2=inputsid2;
            tempadjListNode.weight=inputwgt;

            it->listnode.push_back(tempadjListNode);
            outputnodecount++;


            /*cout<< it->sid1<<"\t"
                << it->listnode.at(0).sid2<<"\t"
                << it->listnode.at(0).weight<<endl;*/

        } else { // graph exist
            //create new node
            adjListNode tempadjListNode;
            tempadjListNode.sid2=inputsid2;
            tempadjListNode.weight=inputwgt;

            it->listnode.push_back(tempadjListNode);
            outputnodecount++;

        } //else

        //create graph by sid2

        it = find_if(adjL.begin(), adjL.end(), FindByName(inputsid2));

        if(it == adjL.end()) { //graph not exist
            adjList tempadjList;
            tempadjList.sid1=inputsid2;

            adjL.push_back(tempadjList); //create new graph

            it = find_if(adjL.begin(), adjL.end(), FindByName(inputsid2));

            //create new node
            adjListNode tempadjListNode;
            tempadjListNode.sid2=inputsid1;
            tempadjListNode.weight=inputwgt;

            it->listnode.push_back(tempadjListNode);
            outputnodecount++;

            /*cout<< it->sid1<<"\t"
                << it->listnode.at(0).sid2<<"\t"
                << it->listnode.at(0).weight<<endl;*/

        } else { // graph exist
            //create new node
            adjListNode tempadjListNode;
            tempadjListNode.sid2=inputsid1;
            tempadjListNode.weight=inputwgt;

            it->listnode.push_back(tempadjListNode);
            outputnodecount++;
        } //else




    } // for


    //checking for missing sid2
    /*
    iter = aList.begin();
    for(iter; iter!= aList.end(); iter++) {
        string inputsid2(iter->sid2);
        vector<adjList>::iterator itercheck = find_if(adjL.begin(), adjL.end(), FindByName(inputsid2));
        if(itercheck == adjL.end()) { //graph not exist
            cout<<"NOT EXIST"<<endl;
            adjList tempadjList;
            tempadjList.sid1=inputsid2;
            adjL.push_back(tempadjList); //create new graph
        }
    }
    */

    //sorting
    sort(adjL.begin(), adjL.end(),compareBy_adjList_sid1 ); //sorting graph

    //sorting notes
    vector<adjList>::iterator itersorting = adjL.begin();
    for(itersorting; itersorting!= adjL.end(); itersorting++) {
        sort(itersorting->listnode.begin(), itersorting->listnode.end(),compareBy_adjListNode_sid2 );


    }


} //createASJlist()


void Graph::createASJlist_wgt() {

    vector<studentPair>::iterator iter = aList.begin();
    for(iter; iter!= aList.end(); iter++) {

        /*cout<< iter->sid1 << "\t"
            << iter->sid2 << "\t"
            << iter-> wgt << endl;*/

        float inputwgt = iter-> wgt;
        string inputsid1(iter->sid1);
        string inputsid2(iter->sid2);

        /*for(int countf=1; iter-> wgt>inputweight; countf++,iter++) {
            cout<<"skiped: "<<iter-> wgt<<endl;
        }*/
        if(iter-> wgt<=inputweight) {

            //create graph by sid1

            vector<adjList>::iterator it = find_if(adjL.begin(), adjL.end(), FindByName(inputsid1));

            if(it == adjL.end()) { //graph not exist
                adjList tempadjList;
                tempadjList.sid1=inputsid1;

                adjL.push_back(tempadjList); //create new graph

                it = find_if(adjL.begin(), adjL.end(), FindByName(inputsid1));

                //create new node
                adjListNode tempadjListNode;
                tempadjListNode.sid2=inputsid2;
                tempadjListNode.weight=inputwgt;

                it->listnode.push_back(tempadjListNode);
                outputnodecount++;


                /*cout<< it->sid1<<"\t"
                    << it->listnode.at(0).sid2<<"\t"
                    << it->listnode.at(0).weight<<endl;*/

            } else { // graph exist
                //create new node
                adjListNode tempadjListNode;
                tempadjListNode.sid2=inputsid2;
                tempadjListNode.weight=inputwgt;

                it->listnode.push_back(tempadjListNode);
                outputnodecount++;

            } //else

            //create graph by sid2

            it = find_if(adjL.begin(), adjL.end(), FindByName(inputsid2));

            if(it == adjL.end()) { //graph not exist
                adjList tempadjList;
                tempadjList.sid1=inputsid2;

                adjL.push_back(tempadjList); //create new graph

                it = find_if(adjL.begin(), adjL.end(), FindByName(inputsid2));

                //create new node
                adjListNode tempadjListNode;
                tempadjListNode.sid2=inputsid1;
                tempadjListNode.weight=inputwgt;

                it->listnode.push_back(tempadjListNode);
                outputnodecount++;


                /*cout<< it->sid1<<"\t"
                    << it->listnode.at(0).sid2<<"\t"
                    << it->listnode.at(0).weight<<endl;*/

            } else { // graph exist
                //create new node
                adjListNode tempadjListNode;
                tempadjListNode.sid2=inputsid1;
                tempadjListNode.weight=inputwgt;

                it->listnode.push_back(tempadjListNode);
                outputnodecount++;

            } //else

        }//skip invalid wgt


    } // for


    //checking for missing sid2
    /*
    iter = aList.begin();
    for(iter; iter!= aList.end(); iter++) {
        string inputsid2(iter->sid2);
        vector<adjList>::iterator itercheck = find_if(adjL.begin(), adjL.end(), FindByName(inputsid2));
        if(itercheck == adjL.end()) { //graph not exist
            cout<<"NOT EXIST"<<endl;
            adjList tempadjList;
            tempadjList.sid1=inputsid2;
            adjL.push_back(tempadjList); //create new graph
        }
    }
    */

    //sorting
    sort(adjL.begin(), adjL.end(),compareBy_adjList_sid1 ); //sorting graph

    //sorting notes
    vector<adjList>::iterator itersorting = adjL.begin();
    for(itersorting; itersorting!= adjL.end(); itersorting++) {
        sort(itersorting->listnode.begin(), itersorting->listnode.end(),compareBy_adjListNode_sid2 );


    }


} //createASJlist_wgt()


void Graph::saveFileASJlist(string outputfilename) {

    outputfilename = "pairs" + outputfilename + ".adj";
    FILE* file = NULL;
    file = fopen( outputfilename.c_str(), "w");

    cout<<"Size (IDs): "<<adjL.size()<<endl;
    cout<<"nodes: "<<outputnodecount<<endl;

    //fprintf(file, "OID\tArrival	Duration\tTimeOut\n");
    int c,cc = 1;

    vector<adjList>::iterator iter = adjL.begin();
    for(iter, c = 1; iter!= adjL.end(); iter++,c++) {
        string tempstring1 = iter->sid1;
        //cout<<tempstring1<<endl;
        fprintf(file, "[%d] %s:\n", c, tempstring1.c_str());


        vector<adjListNode>::iterator iter2 = iter->listnode.begin();
        for(iter2, cc = 1; iter2!= iter->listnode.end(); iter2++,cc++) {
            fprintf(file, "(%d) %s, %f\t", cc, iter2->sid2.c_str(), iter2->weight);

        }
        fprintf(file, "\n");

    }

    fclose(file);

}

void Graph::DFS() {
    string temps;
    int cpmCount = 0;

    // Initial cntCpm
    cntCpm.clear();
    adjListcnt tempnode;
    tempnode.finished=false;
    cntCpm.push_back(tempnode);

    // Initially mark all verices as not visited
    //vector<bool> visited(V, false);
    for (int x = 0; x < adjL.size(); x++) {
        adjL.at(x).visited=false;
    }

    // Create a stack for DFS
    stack<string> sid1stack;

    // Push the current source node.
    for (int i = 0; i < adjL.size(); i++) {

        if(adjL.at(i).visited==false) {
            sid1stack.push(adjL.at(i).sid1);

            while (!sid1stack.empty()) {
                // Pop a vertex from stack and print it
                temps = sid1stack.top();
                sid1stack.pop();

                // Stack may contain same vertex twice. So
                // we need to print the popped item only
                // if it is not visited.
                vector<adjList>::iterator itercheck = find_if(adjL.begin(), adjL.end(), FindByName(temps));
                if(itercheck==adjL.end()) {
                    cout<<"error1"<<endl;
                }

                if (!(itercheck->visited)) {

                    cntCpm.at(cpmCount).sid.push_back(temps); //cout << temps << " ";
                    itercheck->visited = true;
                }

                // Get all adjacent vertices of the popped vertex s
                // If a adjacent has not been visited, then push it
                // to the stack.
                for (int ii = 0; ii < itercheck->listnode.size(); ii++) {


                    vector<adjList>::iterator itercheck2 = find_if(adjL.begin(), adjL.end(), FindByName(itercheck->listnode.at(ii).sid2));


                    if(!(itercheck2->visited)) {
                        sid1stack.push(itercheck->listnode.at(ii).sid2);
                    }


                }//for
            }//while

            sort(cntCpm.at(cpmCount).sid.begin(), cntCpm.at(cpmCount).sid.end(),compareBy_string ); //sorting graph

            cntCpm.push_back(tempnode);
            cpmCount++;

        }//if
    }//for
    cntCpm.erase(cntCpm.end());
    sort(cntCpm.begin(), cntCpm.end(),compareBy_adjListcnt_size ); //sorting graph

}//DFS()

void Graph::saveDFS(string outputfilename) {

    outputfilename = "pairs" + outputfilename + ".cc";
    FILE* file = NULL;
    file = fopen( outputfilename.c_str(), "w");

    cout<<"Size (IDs): "<<adjL.size()<<endl;
    cout<<"nodes: "<<outputnodecount<<endl;

    //fprintf(file, "OID\tArrival	Duration\tTimeOut\n");
    int c,cc = 1;

    vector<adjListcnt>::iterator iter = cntCpm.begin();
    for(iter, c = 1; iter!= cntCpm.end(); iter++,c++) {
        //string tempstring1 = iter->sid1;
        //cout<<tempstring1<<endl;
        fprintf(file, "[%d]: size = %d \n", c,iter->sid.size() );

        for(int iii=0; iii<iter->sid.size(); iii++) {
            //cout<<iter->sid.size()<<endl;
            fprintf(file, "(%d) %s\t", iii+1, iter->sid.at(iii).c_str());


        }
        fprintf(file, "\n");

    }

    fclose(file);

}


void Graph::dijkstra(string inputid) {


    for (int i = 0; i < adjL.size(); i++) {
        adjL.at(i).inf=99999;

    }
    //itercheck = u
    vector<adjList>::iterator itercheck = find_if(adjL.begin(), adjL.end(), FindByName(inputid));

    set< pair<float, string> > vertexSet; //dist, id
    vertexSet.insert(make_pair(0,inputid));
    itercheck->inf=0;

    while (!vertexSet.empty()) {
        pair<float, string> now = *vertexSet.begin();
        vertexSet.erase(vertexSet.begin());

        string vvv = now.second; //id
        float www = now.first;//weight


        vector<adjList>::iterator iter_vvv = find_if(adjL.begin(), adjL.end(), FindByName(vvv));
        //const vector<pair<float, string> > &edges = mp[v];

        for(int ii = 0; ii< iter_vvv->listnode.size(); ii++) {

            vector<adjList>::iterator itercheck_temp = find_if(adjL.begin(), adjL.end(), FindByName(iter_vvv->listnode.at(ii).sid2));
            //float dist[to.first] = itercheck_temp->inf;
            //float to.second = iter_vvv->listnode.at(ii).weight;

            if(www+iter_vvv->listnode.at(ii).weight < itercheck_temp->inf ) {

                vertexSet.erase(make_pair(itercheck_temp->inf,iter_vvv->listnode.at(ii).sid2));
                itercheck_temp->inf = www +iter_vvv->listnode.at(ii).weight;
                vertexSet.insert(make_pair(itercheck_temp->inf,iter_vvv->listnode.at(ii).sid2));



            }//if





        }//for





    }//while
    /*
    for(int iii=0;iii<adjL.size();iii++){
        cout<<endl<<adjL.at(iii).sid1<<": "<<adjL.at(iii).inf<<endl;
    }
    */
    sort(adjL.begin(), adjL.end(),compareBy_adjList_inf ); //sorting graph





}

void Graph::savedijkstra(string outputfilename, string inputidd) {

    outputfilename = "pairs" + outputfilename + ".ds";
    FILE* file = NULL;
    file = fopen( outputfilename.c_str(), "a");

    int ccccc = 1;
    fprintf(file, "\norigin: %s\n", inputidd.c_str());
    for(int iii=0; iii<adjL.size(); iii++) {

        if(!(adjL.at(iii).inf==99999||adjL.at(iii).inf==0)) {
            fprintf(file, "(%d) %s, %f\t", ccccc, adjL.at(iii).sid1.c_str(),adjL.at(iii).inf);
            ccccc++;

        }

    }


    fclose(file);


}

bool check0=false;
bool check1=false;

Graph globtemp;
string globfilename;

void newmode0() {
    check0=true;
    check1=true;
    Graph test;

    string filename;



    while(test.fileexisted==false) {
        cout<<endl<<"Input a file number:"<<endl;
        cin >> filename;
        string readfilename="pairs"+filename+".bin";
        test.readBinary(readfilename);

    }
    test.fileexisted = false;

    cout<<endl<<"Input a real number in (0,1]:"<<endl;
    cin>>test.inputweight;

    test.createASJlist_wgt();

    stringstream ss;
    ss << test.inputweight;
    string s(ss.str());


    filename = filename + "_" + s;

    test.saveFileASJlist(filename);

    test.DFS();
    test.saveDFS(filename);

    globtemp=test;
    globfilename=filename;

}


void newmode1() {

    check1=true;
    if(check0==false) {
        cout<<"There is no graph and choose 0 first."<<endl;
    }

}


void newmode2() {
    if(check1==false) {
        cout<<"### Choose 1 to find connected components. ###"<<endl;
    }

    Graph test;
    test = globtemp;



    string filename = globfilename;
    /*
    cin >> filename;


    string readfilename="pairs"+filename+".bin";
    test.readBinary(readfilename);
    cin>>test.inputweight;

    test.createASJlist_wgt();

    stringstream ss;
    ss << test.inputweight;
    string s(ss.str());


    filename = filename + "_" + s;

    test.saveFileASJlist(filename);*/


    for(int i = 0; i<test.adjL.size(); i++){

        cout<<test.adjL.at(i).sid1<<"\t";



    }


    string inputidd="-1";
    while(true) {
        cout<<endl<<"Input a student ID [0: exit]"<<endl;
        cin>>inputidd;
        if(inputidd=="0")
            break;
        test.dijkstra(inputidd);
        test.savedijkstra(filename,inputidd);



    }


}

int main(void) {

    do {

        cout << endl << "**** Graph data application ***********";
        cout << endl << "* 1.  Create adjacency lists &        *";
        cout << endl << "*     Build connected components      *";
        cout << endl << "* 2.  Find shortest paths by Dijkstra *";
        cout << endl << "***************************************";
        cout << endl << "Input a choice(1, 2)";

        int command ;
        cin >> command;
        if ((command == 0) || (command == 1)|| (command == 2)) {
            if (command == 0) {
                //newmode0();

            }
            if (command==1) {

                newmode0();

            }
            if (command==2) {

                newmode2();

            }
        } else if (!command)
            break;
        else
            cout << endl << "Command does not exist!" << endl;
    } while (true);
    system("pause");
    return 0;
}
