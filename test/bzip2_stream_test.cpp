#include <gtest/gtest.h>

int gauze_main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#include <bzip2stream/bzip2stream.hpp>
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <boost/crc.hpp>

using namespace std;
using namespace bzip2_stream;

// a dummy class
class dummy
{
public:
    friend bool operator==(const dummy& a, const dummy& b);

    dummy(float f_ = 3.14, int i_ = 1)
        : f(f_)
        , i(i_){};
    void reset()
    {
        f = 0;
        i = 0;
    };

    friend ostream& operator<<(ostream& out, dummy const& d)
    {
        out << " " << d.f << " " << d.i;
        return out;
    }
    friend istream& operator>>(istream& in, dummy& d)
    {
        in >> d.f >> d.i;
        return in;
    }

    friend wostream& operator<<(wostream& out, dummy const& d)
    {
        out << L" " << d.f << L" " << d.i;
        return out;
    }
    friend wistream& operator>>(wistream& in, dummy& d)
    {
        in >> d.f >> d.i;
        return in;
    }

protected:
    float f;
    int i;
};

bool operator==(const dummy& a, const dummy& b)
{
    return (a.f == b.f) && (a.i == b.i);
}

TEST(bzip2stream, test_buffer_to_buffer)
{
    boost::crc_16_type crc_in, crc_zip;
    const size_t n = 1024;
    char in_buffer[n] = { '\0' };
    char zip_buffer[n] = { '\0' };

    for (size_t i = 0; i < n - 1; ++i)
        in_buffer[i] = static_cast<char>(48 + i % 48);

    ostringstream out;
    bzip2_ostream zipper(out);

    zipper.write(in_buffer, n);
    if (zipper.fail())
        cerr << "failed to write stream" << endl;

    zipper.zflush();

    istringstream in(out.str());
    bzip2_istream unzipper(in);
    unzipper.read(zip_buffer, n);
    crc_in.process_bytes(in_buffer, n);
    crc_zip.process_bytes(zip_buffer, n);

    ASSERT_EQ(crc_in.checksum(), crc_zip.checksum());
}

TEST(bzip2stream, test_wbuffer_to_wbuffer)
{
    boost::crc_16_type crc_in, crc_zip;
    const size_t n = 128;
    wchar_t in_buffer[n] = { '\0' };
    wchar_t zip_buffer[n] = { '\0' };

    for (size_t i = 0; i < n - 1; ++i)
        in_buffer[i] = static_cast<wchar_t>(48 + i % 48);

    wostringstream out;
    bzip2_wostream zipper(out);

    zipper.write(in_buffer, n);
    if (zipper.fail())
        cerr << "failed to write stream" << endl;

    zipper.zflush();

    wistringstream in(out.str());
    bzip2_wistream unzipper(in);
    unzipper.read(zip_buffer, n);

    crc_in.process_bytes(in_buffer, n * sizeof(wchar_t));
    crc_zip.process_bytes(zip_buffer, n * sizeof(wchar_t));

    ASSERT_EQ(crc_in.checksum(), crc_zip.checksum());
}

TEST(bzip2stream, test_string_string)
{
    // create some test values
    char c_r = '-', c = 'a';
    string s_r = "", s = "std::string";
    double d_r = 0, d = asin(1.0) * 2.0;
    float f_r = 0, f = static_cast<float>(asin(1.0) * 2.0);
    unsigned int ui_r = 0, ui = rand();
    unsigned long ul_r = 0, ul = rand();
    unsigned short us_r = 0, us = rand();
    dummy dum, dum_r(0, 0);

    /*----------------------------------------------------------------------
	
	Testing string to string zipping/unzipping
	
	-------------------------------------------------------------------------*/
    // creating the target zip string, could be a fstream
    ostringstream ostringstream_;
    // creating the zip layer
    bzip2_ostream zipper(ostringstream_);

    // writing data
    zipper << f << " " << d << " " << ui << " " << ul << " " << us << " " << c << " " << dum;
    // zip ostream needs special flushing...
    zipper.zflush();

    // create a stream on zip string
    istringstream istringstream_(ostringstream_.str());
    // create unzipper istream
    bzip2_istream unzipper(istringstream_);

    // unzipping
    unzipper >> f_r >> d_r >> ui_r >> ul_r >> us_r >> c_r >> dum_r;

    ASSERT_EQ(d, d_r);
    ASSERT_EQ(c, c_r);
    ASSERT_EQ(f, f_r);
    ASSERT_EQ(ui, ui_r);
    ASSERT_EQ(ul, ul_r);
    ASSERT_EQ(us, us_r);
    ASSERT_EQ(dum, dum_r);
}

TEST(bzip2stream, test_wstring_wstring)
{
    // create some test values
    double d_r = 0, d = asin(1.0) * 2.0;
    float f_r = 0, f = static_cast<float>(asin(1.0) * 2.0);
    unsigned int ui_r = 0, ui = rand();
    unsigned long ul_r = 0, ul = rand();
    unsigned short us_r = 0, us = rand();
    dummy dum, dum_r(0, 0);

    /*

	Testing wide characters

	*/
    f_r = 0;
    d_r = 0;
    ui_r = ul_r = us_r = 0;
    dum_r.reset();
    // creating the target zip string, could be a fstream
    wostringstream wostringstream_;
    // creating the zip layer
    bzip2_wostream wzipper(wostringstream_);

    // writing data
    wzipper << f << L" " << d << L" " << ui << L" " << ul << L" " << us << L" " << dum;
    // zip ostream needs special flushing...
    wzipper.zflush();

    // create a stream on zip string
    wistringstream wistringstream_(wostringstream_.str());
    // create unzipper istream
    bzip2_wistream wunzipper(wistringstream_);

    // unzipping
    wunzipper >> f_r >> d_r >> ui_r >> ul_r >> us_r >> dum_r;

    ASSERT_EQ(d, d_r);
    ASSERT_EQ(f, f_r);
    ASSERT_EQ(ui, ui_r);
    ASSERT_EQ(ul, ul_r);
    ASSERT_EQ(us, us_r);
    ASSERT_EQ(dum, dum_r);
}

TEST(bzip2stream, test_file_file)
{
    // create some test values
    char c_r = '-', c = 'a';
    string s_r = "", s = "std::string";
    double d_r = 0, d = asin(1.0) * 2.0;
    float f_r = 0, f = static_cast<float>(asin(1.0) * 2.0);
    unsigned int ui_r = 0, ui = rand();
    unsigned long ul_r = 0, ul = rand();
    unsigned short us_r = 0, us = rand();
    dummy dum, dum_r(0, 0);

    /*----------------------------------------------------------------------------
	
	Testing file to file unzipping

	------------------------------------------------------------------------------*/
    f_r = 0;
    d_r = 0;
    ui_r = ul_r = us_r = 0;
    dum_r.reset();

    {
        // creating the target zip string, could be a fstream
        ofstream ofstream_("test.bz", ios::out | ios::binary);
        // creating the zip layer
        bzip2_ostream fzipper(ofstream_);

        // writing data
        fzipper << f << " " << d << " " << ui << " " << ul << " " << us << " " << c << " " << dum;
        // zip ostream needs special flushing...
        fzipper.zflush();
    }

    // create a stream on zip string
    ifstream ifstream_;
    ifstream_.open("test.bz", ios::in | ios::binary);
    if (!ifstream_.is_open())
    {
        cerr << "Could not open file test.zip" << endl;
    }
    // create unzipper istream
    bzip2_istream funzipper(ifstream_);

    // unzipping
    funzipper >> f_r >> d_r >> ui_r >> ul_r >> us_r >> c_r >> dum_r;

    ASSERT_EQ(d, d_r);
    ASSERT_EQ(c, c_r);
    ASSERT_EQ(f, f_r);
    ASSERT_EQ(ui, ui_r);
    ASSERT_EQ(ul, ul_r);
    ASSERT_EQ(us, us_r);
    ASSERT_EQ(dum, dum_r);
}
