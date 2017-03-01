
var binding = require('../build/Release/waitpid');

module.exports = {
	waitpid: function waitpid(pid, options) {
		  //returns the exit code/signal
		  return binding.waitpid(pid, options);
		},
	WNOHANG: binding.WNOHANG,
	WUNTRACED: binding.WUNTRACED,
	WCONTINUED: binding.WCONTINUED
}
