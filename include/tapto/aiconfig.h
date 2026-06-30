// SPDX-License-Identifier: Apache-2.0
// Copyright 2026 Centlake Software AB

#pragma once

// Lightweight settings object for the AI backends. 
class AiConfig {
public:
    int maxOutputTokens() const { return m_maxOutputTokens; }
    int maxToolIterations() const { return m_maxToolIterations; }
    int openaiConnectionTimeoutSeconds() const { return m_connectionTimeoutSeconds; }
    int openaiReadTimeoutSeconds() const { return m_readTimeoutSeconds; }
    bool printCot() const { return m_printCot; }

    void setMaxOutputTokens(int v) { m_maxOutputTokens = v; }
    void setMaxToolIterations(int v) { m_maxToolIterations = v; }
    void setOpenaiConnectionTimeoutSeconds(int v) { m_connectionTimeoutSeconds = v; }
    void setOpenaiReadTimeoutSeconds(int v) { m_readTimeoutSeconds = v; }
    void setPrintCot(bool v) { m_printCot = v; }

private:
    int m_maxOutputTokens = 16000;
    int m_maxToolIterations = 50;
    int m_connectionTimeoutSeconds = 30;
    int m_readTimeoutSeconds = 300;
    bool m_printCot = true; // surface intermediate reasoning/text to the terminal
};
