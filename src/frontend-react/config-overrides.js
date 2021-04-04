/**
 * Based on https://github.com/rjwats/esp8266-react/blob/master/interface/config-overrides.js
 */
const { WebpackManifestPlugin } = require('webpack-manifest-plugin');
const WorkboxWebpackPlugin = require('workbox-webpack-plugin');
const MiniCssExtractPlugin = require('mini-css-extract-plugin');
const rewireCompressionPlugin = require('react-app-rewire-compression-plugin');

const path = require('path');
const fs = require('fs');

module.exports = function override (config, env) {
    if (env === 'production') {
        // rename the ouput file, we need it's path to be short, for embedded FS
        config.output.filename = 'js/[id].[chunkhash:4].js';
        config.output.chunkFilename = 'js/[id].[chunkhash:4].js';

        // take out the manifest and service worker plugins
        config.plugins = config.plugins.filter(plugin => !(plugin.constructor.name === 'ManifestPlugin'));

        // shorten css filenames
        const miniCssExtractPlugin = config.plugins.find((plugin) => (plugin.constructor.name === 'MiniCssExtractPlugin'));

        miniCssExtractPlugin.options.filename = "css/[id].[contenthash:4].css";
        miniCssExtractPlugin.options.chunkFilename = "css/[id].[contenthash:4].c.css";

        // config.plugins.push(rewireCompressionPlugin(config, env, {
        //     test: /\.(css|js)$/i,
        //     cache: true
        // }))
    }
    return config;
}
