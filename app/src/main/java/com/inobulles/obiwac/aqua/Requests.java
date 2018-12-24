package com.inobulles.obiwac.aqua;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;

class RequestResponse {
    public String text;
    public int    code;

    RequestResponse(String text, int code) {
        this.text = text;
        this.code = code;

    }

}

public class Requests {
    private static RequestResponse __get(String url_string) throws IOException {
        URL               url;
        HttpURLConnection connection;
        BufferedReader    reader;

        try {
            url = new URL(url_string);

        } catch (MalformedURLException exception) {
            exception.printStackTrace();
            throw new IOException();

        } try {
            connection = (HttpURLConnection) url.openConnection();

        } catch (IOException exception) {
            exception.printStackTrace();
            throw new IOException();

        } try {
            connection.setRequestMethod("GET");

        } catch (ProtocolException exception) {
            exception.printStackTrace();
            throw new IOException();

        }

        connection.setDoOutput      (true);
        connection.setConnectTimeout(5000);
        connection.setReadTimeout   (5000);

        try {
            connection.connect();

        } catch (IOException exception) {
            exception.printStackTrace();
            throw new IOException();

        } try {
            reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));

        } catch (IOException exception) {
            exception.printStackTrace();
            throw new IOException();

        }

        StringBuilder content = new StringBuilder();

        String line;
        while ((line = reader.readLine()) != null) {
            content.append(line).append("\n");

        }

        return new RequestResponse(content.toString(), connection.getResponseCode());

    }

    public static RequestResponse get(String url_string) {
        try {
            return __get(url_string);

        } catch (IOException exception) {
            exception.printStackTrace();
            return null;

        }

    }

}
