{
  "targets": [
    {
      "target_name": "waitpid",
      "sources": [ "src/waitpid.cc" ],
      "include_dirs" : [
 	 		  "<!(node -e \"require('nan')\")"
			]
    }
  ],
}
