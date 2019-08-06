# MyTanimoto

The MyTanimoto library adds to MySQL and MariaDB the ability to compute
tanimoto scores. It is based on [Mychem](http://mychem.sourceforge.net/) and
is released under the GNU GPL license.

## Installation

Note: The installation has only been tested on GNU/Linux.

### Requirements

The following software should be installed before compiling the library:
* On RedHat or CentOS:
  ```bash
  # yum install mariadb-devel
  ```
* On Ubuntu or Debian
  ```bash
  # sudo apt install libmariadbclient-dev
  ```

### Build and installation

```bash
# tar -xfzv mychem-0.9.4.tar.gz
# cd mytanimoto
# make
```

Then, you have to copy the mytanimoto.so file into the MariaDB plugin
directory. This directory is given by the value of the plugin_dir system
variable:
```bash
# mysql -u root -p --execute "SHOW VARIABLES LIKE 'plugin_dir';"
+---------------+--------------------------+
| Variable_name | Value                    |
+---------------+--------------------------+
| plugin_dir    | /usr/lib64/mysql/plugin/ |
+---------------+--------------------------+
# sudo cp mytanimoto.so /usr/lib64/mysql/plugin/
```

Once the libraries are installed, you have to create the SQL function. For
Unix like systems (Linux, BSD, Mac OS X), use the following command:
```bash
# mysql -u root -p < import.sql
```

## License

MyTanimoto is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 2 of the License, or
(at your option) any later version. Full details
can be found in the [LICENSE](LICENSE) file.
