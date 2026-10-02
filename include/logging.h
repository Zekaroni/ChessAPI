#include "utils.h"

// NOTE FOR ALEX:
//     To use the debug functions loggingHelper.<func_name>(<params>)
//     can be called anywhere.

/// @brief Custom logging class for this project
class Logging
{
private:
    std::ofstream debugFile;
    bool _logStartupTime = false;
    bool _logMemoryUsage = false;
    bool _globalLog      = true;
    std::chrono::_V2::steady_clock::time_point _startupStartTime;
    std::chrono::_V2::steady_clock::time_point _startupEndTime;
    
    
public:
    Logging()
    {
        std::string debugFilePath = "./debug/output.txt";
        initDebugFile(debugFilePath);
    };
    void globalLog(bool log){_globalLog = log;};
    void logMemoryUsage(bool log){_logMemoryUsage = log;};

    void startStartupTimer(){_startupStartTime = Clock::now();};
    void endStartupTimer()  {_startupEndTime   = Clock::now();};
    
    bool logStartupTime(){return _logStartupTime;};
    void logStartupTime(bool log){_logStartupTime = log;};
    bool logStartupTimer(bool toFile = true)
    {
        if (toFile && !debugFile.is_open()) return false;
        std::ostream& output = toFile ? static_cast<std::ostream&>(debugFile) : std::cout;
        output << "Total startup time: " <<
                std::chrono::duration_cast<std::chrono::milliseconds>(_startupEndTime - _startupStartTime).count() <<
                "ms";
        output.flush();
        return output.good(); // true if good
        
    }


    bool debug_pieces(piece_t* boardState, bool toFile = true)
    {
        /*
            This function will print or write to the debug file
            the integer representation of each piece at every
            psotition formatted like the chess board.

            returns true if logs succesfully
        */
        if (toFile && !debugFile.is_open()) return false;
        std::ostream& output = toFile ? static_cast<std::ostream&>(debugFile) : std::cout;

        for (int i = 63; i >= 0; i--)
        {
            output << (int)boardState[i];
            if (i % 8 == 0) output << '\n';
        }

        output.flush();
        return output.good(); // true if good
    }

    bool debug_bitboard_array(bitboard_t* bitboardArray, int size, bool toFile = true)
    {
        /*
            This function outputs all the bitboards in an array
            in integer form to selected output.

            returns true if logs succesfully
        */
        if (toFile && !debugFile.is_open()) return 0;
        std::ostream& output = toFile ? static_cast<std::ostream&>(debugFile) : std::cout;

        for (int i = 0; i < size; i++)
        {
            output << bitboardArray[i] << '\n';
        }

        output.flush();
        return output.good();
    }

    bool debug_bitboard(bitboard_t value, bool toFile = true)
    {
        /*
            This funciton outputs the bitboard formatted like
            a chess board.

            returns true if logs succesfully
        */
        if (toFile && !debugFile.is_open()) return false;
        std::ostream& output = toFile ? static_cast<std::ostream&>(debugFile) : std::cout;

        for (int i = 63; i >= 0; i--)
        {
            output << ((value >> i) & 1);
            if (i % 8 == 0) output << '\n';
        }
        output << '\n';

        output.flush();
        return output.good();
    }

    bool initDebugFile(std::string filePath)
    {
        debugFile.open(filePath.c_str());
        if (!debugFile)
        {
            std::cerr << "Failed to open debug file.\n"; 
            return 0;
        }
        return 1;
    }

    bool outputToDebugFile(std::string output)
    {
        if (!debugFile.is_open()) return false;
        debugFile << output.c_str();
        return debugFile.good();
    }

    bool closeDebugFile()
    {
        if(!debugFile.is_open()) return 0;
        debugFile.close();
        return true;
    }


    bool streamToTerminal(std::string output)
    {
        /*
            This functions streams the output to one line.
            NOTE: This breaks if there are new lines
        */
        std::cout << output << '\r';
        return true;
    }

    bool printToTerminal(std::string output)
    {
        if (_globalLog)
        {
            std::cout << output.c_str();
            return true;
        }
        return false;
    }
};