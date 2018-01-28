=========
Dodos Day
=========

Dodo(om)s Day is about prolonging the inevitable...

.. image:: assets/screenshots/ingame.png

Run
---

To run the project on Linux, you will need to install `SFML
<https://www.sfml-dev.org/>`_, gcc/clang and cmake.

Then ``git clone`` the project and apply the tmxlite patch:

.. code-block:: shell

	git clone https://github.com/qtux/ggj18.git
	cd ggj18
	git apply patch_tmxlite.diff

Now you can compile and run the project:

.. code-block:: shell

	make
	./build/run

Links
-----

https://globalgamejam.org/2018/games/dodos-day

License
-------

This project is licensed under The zlib/libpng License (Zlib).
