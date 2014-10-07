#ifndef __CONFIGPARSER_HPP__
#define __CONFIGPARSER_HPP__

#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <map>

typedef std::map<std::string, std::string> Section;

class ConfigParser{
public:
    ConfigParser(){}
    ~ConfigParser(){}
    void load(const std::string& filename)
    {
        std::ifstream infile(filename.c_str());
        if ( !infile ) {
            std::cout << "Can't open file " << filename << ".\n";
            exit(1);
        }
        std::string line;
        std::string section_name = "";

        while ( infile.peek() != EOF ) {
            std::getline(infile, line);
            if ( *line.rbegin() == '\n' ) {
                line.erase(line.end() - 1);
                if ( *line.rbegin() == '\r' ) {
                    line.erase(line.end() - 1);
                }
            } else if ( *line.rbegin() == '\r' ) {
                line.erase(line.end() - 1);
            }

            if ( line[0] == '\n' || line[0] == '#' ) {
                continue;
            }
            if ( line[0] == '[' ) {
                if ( line[line.size() -1] != ']' ) {
                    std::cout << "Configure file is not correct, section error."
                              << std::endl;
                    exit(1);
                }
                section_name = line.substr(1, line.size() - 2);
                Section section;
                m_sectionMap[section_name] = section;
                continue;
            }
            if ( section_name.empty() ) {
                std::cout << "Configure file is not correct, no section name."
                          << std::endl;
                exit(1);
            }
            int eqpos;
            eqpos = line.find('=');
            if ( eqpos == line.npos ) {
                continue;
            }
            if ( line.substr(0, eqpos).empty() ) {
                std::cout << "Configure file is not correct, syntax error near '"
                          << line << "'." << std::endl;
                exit(1);
            }
            m_sectionMap[section_name].insert(
                make_pair(line.substr(0, eqpos), 
                          line.substr(eqpos + 1, line.size() - eqpos - 1)));
        }
    }

    std::string get(const std::string& section, const std::string& key)
    {
        std::map<std::string, Section>::iterator mit = m_sectionMap.find(section);
        if ( mit != m_sectionMap.end() ) {
            Section::iterator sit = mit->second.find(key);
            if ( sit != mit->second.end() ) {
                return sit->second;
            } else {
                std::cout << "No this key " << "[" << key << "]\n.";
                exit(1);
            }
        } else {
            std::cout << "No this section " << "[" << section << "].\n";
            exit(1);
        }
    }

    void getAll()
    {
        for ( std::map<std::string, Section>::iterator mit = m_sectionMap.begin();
              mit != m_sectionMap.end(); ++mit ) {
            std::cout << mit->first << std::endl;
            for ( Section::iterator sit = mit->second.begin();
                  sit != mit->second.end(); ++sit ) {
                std::cout << sit->first << " " << sit->second << std::endl;
            }
        }
    }
private:
    std::map<std::string, Section> m_sectionMap;
}; // end class ConfigParser

#endif