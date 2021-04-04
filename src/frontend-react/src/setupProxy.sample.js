// Copy this to setupProxy.js and change the ESPUrl variable to your local device
// Install http-proxy-middleware
// Restart the React devserver

const ESPUrl = 'http://192.168.1.123'

const { createProxyMiddleware } = require('http-proxy-middleware');

module.exports = function (app) {
    app.use('/api', createProxyMiddleware({ target: ESPUrl, changeOrigin: true, }))
}