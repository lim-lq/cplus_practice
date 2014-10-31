#ifndef __CONFIGUREPARSER_HPP__
#define __CONFIGUREPARSER_HPP__

#include <fstream>
#include <iostream>
#include <string>
#include <map>


namespace wind
{

typedef std::map<std::string, std::string> KeyValue;
std::string empty_str = "";

class ConfigureParser
{
public:
    ConfigureParser(){}
    ~ConfigureParser(){}
    void read(const std::string& filename) {
        std::ifstream infile(filename.c_str());
        std::string line;
        std::string section;

        while ( infile.peek() != EOF ) {
            std::getline(infile, line);
            // Erase \r \n after the end;
            if ( *line.rbegin() == '\n' ) {
                line.erase(line.end() - 1);
            }
            if ( *line.rbegin() == '\r' ) {
                line.erase(line.end() - 1);
            }

            if ( line.empty() || line[0] == '\n' ) {
                continue;
            }
            if ( '[' == line[0] ) {
                if (  ']' ==  line[line.size() - 1] ) {
                    section = line.substr(1, line.size() - 2);
                    KeyValue keyvalue;
                    m_sections[section] = keyvalue;
                } else {
                    continue;
                }
            }
            std::size_t pos = line.find('=');
            if ( 0 == pos || line.size() -1 == pos || std::string::npos == pos ) {
                continue;
            }
            std::string key = line.substr(0, pos);
            std::string value = line.substr(pos + 1);
            m_sections[section][key] = value;
        }
    }

    const std::string& get(const std::string& section, const std::string& key)
    {
        if ( m_sections.find(section) != m_sections.end() ) {
            if ( m_sections[section].find(key) != m_sections[section].end() ) {
                return m_sections[section][key];
            }
        }
        return empty_str;
    }

    friend std::ostream& operator << (std::ostream& os, ConfigureParser& config);

private:
    std::map<std::string, KeyValue> m_sections;
}; // end class ConfigureParser

std::ostream& operator << (std::ostream& os, ConfigureParser& config)
{
    for ( std::map<std::string, KeyValue>::iterator it = config.m_sections.begin();
          it != config.m_sections.end(); ++it ) {
        if ( it != config.m_sections.begin() ) {
            os << std::endl;
        }
        os << "[" << it->first << "]" << std::endl;
        for ( KeyValue::iterator sit = it->second.begin(); sit != it->second.end(); ++sit ) {
            os << sit->first << " = " << sit->second;
            if ( (++sit) != it->second.end() ) {
                os << std::endl;
            }
            sit--;
        }
    }
    return os;
}

} // end namespace wind
#endif