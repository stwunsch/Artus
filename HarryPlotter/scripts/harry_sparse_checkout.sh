#!/bin/bash

git clone https://github.com/KIT-CMS/Artus.git -n
cd Artus
git remote add -f Artus https://github.com/KIT-CMS/Artus.git
git config core.sparsecheckout true
echo "/HarryPlotter/" >> .git/info/sparse-checkout
echo "/Utility/python/" >> .git/info/sparse-checkout
echo "/Utility/scripts/" >> .git/info/sparse-checkout
echo "/Configuration/scripts/ini_artus_python_standalone.sh" >> .git/info/sparse-checkout
echo "Example/scripts/generateCorrelatedGaussians.py" >> .git/info/sparse-checkout
git pull
git read-tree --empty
git read-tree -mu HEAD
cd -
