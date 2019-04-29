## Installation with a Package Manager

The OGSLib package is available on the recent versions of *Ubuntu*.
```bash
$ sudo apt update
$ sudo apt install software-properties-common
$ sudo add-apt-repository ppa:open5gs/latest
$ sudo apt update
$ sudo apt install ogslib-dev
```

The OGSLib package is also available on [OBS](https://build.opensuse.org/package/show/home:acetcom:open5gs:snapshot/ogslib). First, install the authentication key as shown below.

```bash
$ sudo apt update
$ sudo apt install wget gnupg
$ wget https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.04/Release.key
$ sudo apt-key add Release.key
```

In Debian 9.0(stretch), you can install it as follows:

```bash
$ sudo sh -c "echo 'deb https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Debian_9.0/ ./' > /etc/apt/sources.list.d/open5gs.list"
$ sudo apt update
$ sudo apt install ogslib-dev
```

Other Linux distributions can be installed by changing the path.

```
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Debian_9.0/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/Raspbian_9.0/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_16.04/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_17.10/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.04/
https://download.opensuse.org/repositories/home:/acetcom:/open5gs:/latest/xUbuntu_18.10/
```

[Martin Hauke](https://build.opensuse.org/user/show/mnhauke) packaged NextEPC for *openSUSE* on [OBS](https://build.opensuse.org/package/show/home:mnhauke:nextepc/ogslib).

```bash
$ sudo zypper addrepo -f obs://home:mnhauke:nextepc home:mnhauke:nextepc
$ sudo zypper install ogslib
```

## Building OGSLib

There is no dependencies for building the source code.

Git clone as below:

```
$ git clone https://github.com/open5gs/ogslib
```

To compile with autotools:

```bash
$ cd ogslib
$ autoreconf -iv
$ ./configure --prefix=`pwd`/install
$ make -j `nproc`
```

Check whether the compilation is correct.
```bash
$ make check
```

## Support

Problem with OGSLib can be filed as [issues](https://github.com/open5gs/ogslib/issues) in this repository. 

## Contributing

OGSLib is a pure/classic FOSS project, open to contributions from anyone.

[Pull requests](https://github.com/open5gs/ogslib/pulls) are always welcome, and I appreciates any help the community can give to help make OGSLib better.

Do you want to be a committer? Please [send me an email](mailto:acetcom@gmail.com). You will be added as a committer to this project. However, if someone consistently causes difficulties with these source repositories due to poor behavior or other serious problems then commit access may be revoked.

## License

OGSLib source files are made available under the terms of the GNU Affero General Public License (GNU AGPLv3).

When you contribute code for OGSLib, the same license applies.

