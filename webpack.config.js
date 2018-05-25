const path = require('path');

module.exports = {
  mode: 'development',
  entry: './lib/es6/src/Main.bs.js',
  output: {
    path: path.resolve(__dirname, 'dist'),
    filename: 'main.js'
  },
  module: {
    rules: [
      { test: /\.js$/, exclude: /node_modules/, loader: "babel-loader" }
    ]
  }
};
