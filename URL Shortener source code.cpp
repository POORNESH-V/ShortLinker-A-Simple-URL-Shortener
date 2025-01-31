#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

class URLShortener {
private:
    unordered_map<string, string> urlMap;
    const string filename = "url_mapping.dat";

public:
    // Constructor to load existing URL mappings from file
    URLShortener() {
        ifstream infile(filename, ios::binary);
        if (infile.is_open()) {
            string shortUrl, longUrl;
            while (getline(infile, shortUrl) && getline(infile, longUrl)) {
                urlMap[shortUrl] = longUrl;
            }
            infile.close();
        }
    }

    // Destructor to save the current mappings back to the file
    ~URLShortener() {
        ofstream outfile(filename, ios::binary);
        if (outfile.is_open()) {
            for (auto &entry : urlMap) {
                outfile << entry.first << endl;
                outfile << entry.second << endl;
            }
            outfile.close();
        }
    }

    // Generate a unique short URL using the current timestamp
    string generateShortUrl(const string &longUrl) {
        // Hash the long URL using current time and simple manipulation
        stringstream ss;
        time_t now = time(0);
        ss << hex << now;
        string shortUrl = ss.str().substr(0, 6); // First 6 characters of hex time
        return shortUrl;
    }

    // Shorten the URL and store it in the map
    string shortenUrl(const string &longUrl) {
        string shortUrl = generateShortUrl(longUrl);
        while (urlMap.find(shortUrl) != urlMap.end()) {
            shortUrl = generateShortUrl(longUrl); // Regenerate if collision occurs
        }
        urlMap[shortUrl] = longUrl;
        return shortUrl;
    }

    // Retrieve the original URL using the short URL
    string retrieveUrl(const string &shortUrl) {
        if (urlMap.find(shortUrl) != urlMap.end()) {
            return urlMap[shortUrl];
        } else {
            return "URL not found!";
        }
    }

    // Display all mappings (for testing purposes)
    void displayAll() {
        for (auto &entry : urlMap) {
            cout << "Short URL: " << entry.first << " -> Long URL: " << entry.second << endl;
        }
    }
};

int main() {
    URLShortener shortener;
    int choice;
    string longUrl, shortUrl;

    do {
        cout << "\n:: URL Shortener ::\n";
        cout << "1. Shorten a URL\n";
        cout << "2. Retrieve original URL\n";
        cout << "3. Display all URL mappings\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter the long URL: ";
            cin.ignore();
            getline(cin, longUrl);
            shortUrl = shortener.shortenUrl(longUrl);
            cout << "Shortened URL: " << shortUrl << endl;
            break;
        case 2:
            cout << "Enter the short URL: ";
            cin >> shortUrl;
            cout << "Original URL: " << shortener.retrieveUrl(shortUrl) << endl;
            break;
        case 3:
            shortener.displayAll();
            break;
        case 4:
            cout << "Exiting..." << endl;
            break;
        default:
            cout << "Invalid choice! Try again." << endl;
        }
    } while (choice != 4);

    return 0;
}
