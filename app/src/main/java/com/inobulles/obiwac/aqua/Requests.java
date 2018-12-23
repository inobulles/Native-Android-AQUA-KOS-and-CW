package com.inobulles.obiwac.aqua;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.ProtocolException;
import java.net.URL;

public class Requests {
    public Requests() throws IOException {
        URL url;
        HttpURLConnection connection;
        BufferedReader reader;

        try {
            url = new URL("https://www.google.com/");

        } catch (MalformedURLException exception) {
            exception.printStackTrace();
            return;

        } try {
            connection = (HttpURLConnection) url.openConnection();

        } catch (IOException exception) {
            exception.printStackTrace();
            return;

        } try {
            connection.setRequestMethod("GET");

        } catch (ProtocolException exception) {
            exception.printStackTrace();
            return;

        }

        connection.setDoOutput      (true);
        connection.setConnectTimeout(5000);
        connection.setReadTimeout   (5000);

        try {
            connection.connect();

        } catch (IOException exception) {
            exception.printStackTrace();
            return;

        } try {
            reader = new BufferedReader(new InputStreamReader(connection.getInputStream()));

        } catch (IOException exception) {
            exception.printStackTrace();
            return;

        }

        String content = "", line;
        while ((line = reader.readLine()) != null) {
            content += line + "\n";

        }

    }

}
