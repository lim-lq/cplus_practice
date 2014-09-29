#ifndef __CONFIGPARSER_HPP__
#define __CONFIGPARSER_HPP__

#include <stdlib.h>
#include <iostream>
#include <string>
#include <map>
#include <fstream>

typedef std::map<std::string, std::string> Section;
typedef std::map<std::string, Section> Sections;

class ConfigParser{
public:
    ConfigParser(){}

    bool read(const std::string& file)
    {
        std::string line;
        std::ifstream instream(file.c_str());
        Section* curSection;
        if ( !instream.is_open() ) {
            std::cout << "Open config file error." << std::endl;
        }
        while ( instream.peek() != EOF ) {
            std::getline(instream, line);

            if ( *line.rbegin() == '\n' || *line.rbegin() == '\r' ) {
                line.erase(line.end() - 1);
            }
            if ( line[0] == '\n' || line[0] == '#' || line.empty() ) {
                continue;
            }
            std::cout << "Read line: " << line << std::endl;
            if ( line[0] == '[' ) {
                if ( line[line.length() - 1] != ']' ) {
                    std::cout << "parser file error, the config file has some problems."
                              << "[section syntax error]"
                              << std::endl;
                    return false;
                }
                Section section;
                std::string section_key = line.substr(1, line.length() - 2);
                m_Sections[section_key] = section;
                curSection = &m_Sections[section_key];
            } else {
                size_t pos = line.find('=');
                if ( pos == std::string::npos ) {
                    continue;
                }
                std::cout << pos << std::endl;
                std::cout << line.length() - 1 << std::endl;
                std::string key;
                std::string value;

                key = line.substr(0, pos);
                if ( key.empty() ) {
                    std::cout << "parser file error, the config file has some problems."
                              << "[No key]"
                              << std::endl;
                    return false;
                }
                value = line.substr(pos + 1, line.length() - 1);
                if ( value.empty() ) {
                    continue;
                }
                (*curSection)[key] = value;
            }
        }
        return true;
    }

    void getAll()
    {
        for ( Sections::iterator sit = m_Sections.begin(); sit != m_Sections.end(); ++sit ) {
            std::cout << "[" << sit->first << "]" << std::endl;
            for ( Section::iterator it = sit->second.begin(); it != sit->second.end(); ++it ) {
                std::cout << it->first << " " << it->second << std::endl;
            }
        }
    }

    std::string get(const std::string& section, const std::string& key)
    {
        Sections::iterator sit = m_Sections.find(section);
        if ( sit != m_Sections.end() ) {
            Section::iterator it = sit->second.find(key);
            if ( it != sit->second.end() ) {
                return it->second;
            } else {
                std::cout << "section has no key " << key << std::endl;
            }
        } else {
            std::cout << "No section " << section << std::endl;
        }
        return "";
    }

private:
    Sections m_Sections;
};

#endif