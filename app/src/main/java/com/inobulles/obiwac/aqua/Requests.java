package com.inobulles.obiwac.aqua;

import android.os.StrictMode;

import java.io.*;
import java.net.URL;

import javax.net.ssl.HttpsURLConnection;

class RequestResponse {
    public byte[] text;
    public long   length;
    public int    code;

    RequestResponse(int code, byte[] text, long length) {
        this.text   = text;
        this.length = length;
        this.code   = code;

    }

}

public class Requests {
    public static String USER_AGENT = "Mozilla/5.0 Android AQUA/3.0";

    private static RequestResponse __get(String url_string) throws IOException {
    	URL url = new URL(url_string);
        HttpsURLConnection connection = (HttpsURLConnection) url.openConnection();

        connection.setConnectTimeout(5000); // 5 second timeout
        connection.setReadTimeout(5000);

        connection.setRequestMethod("GET");
        connection.setRequestProperty("User-Agent", USER_AGENT);

        ByteArrayOutputStream baos = new ByteArrayOutputStream();
        InputStream stream = (InputStream) connection.getContent();
        byte[] chunk = new byte[4096]; // read in 4kb chunks

        int n;
        while ((n = stream.read(chunk)) > 0) {
            baos.write(chunk, 0, n);

        }

        stream.close();
        USER_AGENT = "Mozilla/5.0 Android AQUA/3.0"; // reset ua
        return new RequestResponse(connection.getResponseCode(), baos.toByteArray(), baos.size());

    }

    public static RequestResponse get(String url_string) {
        StrictMode.setThreadPolicy(new StrictMode.ThreadPolicy.Builder().permitAll().build()); /// FIXME This should not be here. I'm doing this for now, because the ZVM can't anyway do anything while still requesting, but this isn't a viable long-term solution

        try {
            return __get(url_string);

        } catch (Exception exception) {
            exception.printStackTrace();
            return new RequestResponse(0, null, 0);

        }

    }

}
