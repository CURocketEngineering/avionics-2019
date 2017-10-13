float lat = 0.0;
float lon = 0.0;

const char * getField(const char * sentence, char * buf, unsigned short len) {
    unsigned short idx = 0;

    while (idx < len - 1) {
        if (*sentence == ',') {
            sentence++;
            break;
        }

        buf[idx] = *sentence;

        sentence++;
        idx++;
    }

    buf[idx] = '\0';

    return sentence;
}

void readLine(Stream & stream, char * buf, unsigned short len, bool crlf=true) {
    unsigned short idx = 0;
    int c;

    while (idx < len - 1) {
        while(!stream.available());

        c = stream.read();

        if (crlf) {
            if (c == '\r') {
                continue;
            }
        }

        if (c == '\n') {
            break;
        }

        buf[idx] = c;

        idx++;
    }

    buf[idx] = '\0';
}

void setup() {
    pinMode(13, OUTPUT);
    digitalWrite(13, LOW);

    Serial.begin(9600);
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

        // store data
        lat = llat, lon = llon;
    }

    if (fabs(lat) > 0.1 && fabs(lon) > 0.1)
      digitalWrite(13, HIGH);
}
