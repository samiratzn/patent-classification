#include <pugixml.hpp>
#include <sqlite3x-master/sqlite3x.hpp>
#include <iostream>
#include <string>
#include <thread>
#include <future>
#include <vector>
#include <boost/program_options.hpp>

#include <xmlpatentparser.hpp>

using namespace std;
using namespace sqlite3x; // because it uses prefixes
namespace po = boost::program_options;


/**
 * Parse each of the XML files and insert them into the database
 */
int main(int argc, const char * argv[]) {

    // Option Management

    po::options_description visible("Options");
    visible.add_options()
            ("help", "produce help message")
            ("database,d", po::value<string>()->default_value("patent.db")->value_name("filename"), "Patent sqlite database");

    po::options_description actual;
    actual.add(visible).add_options()
            ("command", po::value< string >(), "Run mode")
            ("input-filename", po::value< vector<string> >(), "Input XML files, from CLEF or USPTO");


    po::positional_options_description p;
    p.add("command", 1);
    p.add("input-filename", -1);


    po::variables_map varmap;
    po::store(po::command_line_parser(argc, argv).
            options(actual).positional(p).run(), varmap);
    po::notify(varmap);


    if (varmap.count("help")) {
        cerr << "Usage: patent store <document> [document ..]" << endl
             << visible << endl;
        return 1;
    }

    if (not (varmap.count("command") && varmap["command"].as<string>() == "store") ) {
        cerr << "Invalid command. Available commands: 'store'" << endl
             << "Usage: patent store <document> [document ..]" << endl
             << visible << endl;
        return 1;
    }

    if (not (varmap.count("input-filename"))) {
        cerr << "Missing input files." << endl
             << "Usage: patent store <document> [document ..]" << endl
             << visible << endl;
        return 1;
    }

    
    // Database setup
    sqlite3_connection conn(varmap["database"].as<string>().c_str());
    conn.setbusytimeout(60000);
    conn.executenonquery("PRAGMA synchronous=0;");
    sqlite3_command insert_patent(conn, "INSERT OR REPLACE INTO patents (id, abstract, description, ipc, title, claims) VALUES (?, ?, ?, ?, ?, ?);");
    sqlite3_command insert_log(conn, "INSERT INTO log (id, filename, success, message) VALUES (?, ?, ?, ?);");
    

    // Load each patent file
    for (string filename : varmap["input-filename"].as< vector<string> >()) {
        // Load each included patent
        for (XMLPatentParser& p : XMLPatentParser::parseXml(filename)) {
            insert_log.bind(1, p.getId());
            insert_log.bind(2, filename);

            if(p.success()) {
                insert_patent.bind(1, p.getId());
                insert_patent.bind(2, p.getAbstract());
                insert_patent.bind(3, p.getDescription());
                insert_patent.bind(4, p.getIPC());
                insert_patent.bind(5, p.getTitle());
                insert_patent.bind(6, p.getClaims());
                insert_patent.executenonquery();
                insert_log.bind(3, 1);
                insert_log.bind(4, "");
                //cout << "Parse succeeded on " << filename << endl;
            } else {
                insert_log.bind(3, 0);
                insert_log.bind(4, p.getErrorLog());
                //cout << "Parse failed on " << filename << endl;
            }
            insert_log.executenonquery();
        }
        cout << "Parsed " << filename << endl;
    }
}
