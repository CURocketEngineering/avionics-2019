#define ADDR 0x13

#include <Wire.h>

float lat = 0.0;
float lon = 0.0;

const char * getField(const char * sentence, char * buf, unsigned short len) {
    unsigned short idx = 0;

    while (idx < len - 1) {
        if (*sentence == ',')
            break;

        buf[idx] = *sentence;

        sentence++;
    }

    buf[idx] = '\0';

    return sentence;
}

void readLine(Stream & stream, char * buf, unsigned short len, bool crlf=true) {
    unsigned short idx = 0;
    char c;

    while (idx < len - 1) {
        c = stream.read();

        if (crlf) {
            if (c == '\r') {
                stream.read();
                break;
            }
        }
        else {
            if (c == '\n') {
                break;
            }
        }

        buf[idx] = c;

        idx++;
    }

    buf[idx] = '\0';
}

void request() {
    static union data_u {
        float lat, lon;
        char bytes[8];
    } data;

    data.lat = lat;
    data.lon = lon;

    Wire.write(data.bytes, 8);
}

void setup() {
    Serial.begin(9600);

    Wire.begin(ADDR);
    Wire.onRequest(request);
}

void loop() {
    static char sentence[80];
    static char field[20];

    static float llat;
    static float llon;

    const char * ptr = sentence;

    readLine(Serial, sentence, sizeof(sentence));

    // id
    ptr = getField(ptr, field, sizeof(field));

    if (strcmp(field, "$GPRMC") == 0) {
        // time
        ptr = getField(ptr, field, sizeof(field));

        // status
        ptr = getField(ptr, field, sizeof(field));

        // lat
        ptr = getField(ptr, field, sizeof(field));
        llat = atof(field);

        // n/s
        ptr = getField(ptr, field, sizeof(field));
        if (field[0] == 'S')
            llat = -llat;

        // lon
        ptr = getField(ptr, field, sizeof(field));
        llon = atof(field);

        // e/w
        ptr = getField(ptr, field, sizeof(field));
        if (field[0] == 'W')
            llon = -llon;

        lat = llat, lon = llon;
    }
}
