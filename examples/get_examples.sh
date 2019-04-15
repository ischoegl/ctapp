# get two examples
wget https://raw.githubusercontent.com/Cantera/cantera/master/interfaces/cython/cantera/examples/onedim/burner_flame.py

mv burner_flame.py cloned_flame.py
sed -i 's/burner_flame/cloned_flame/g' cloned_flame.py
sed -i 's/ct.BurnerFlame/ctapp.ClonedFlame/g' cloned_flame.py
sed -i 's/import cantera as ct/import cantera as ct\nimport ctapp/g' cloned_flame.py

cp cloned_flame.py new_flame.py
sed -i 's/cloned_flame/new_flame/g' new_flame.py
sed -i 's/ctapp.ClonedFlame/ctapp.NewFlame/g' new_flame.py

wget https://raw.githubusercontent.com/Cantera/cantera/master/interfaces/cython/cantera/examples/reactors/reactor1.py

mv reactor1.py cloned_reactor.py
sed -i 's/ct.IdealGasConstPressureReactor/ctapp.ClonedReactor/g' cloned_reactor.py
sed -i 's/import cantera as ct/import cantera as ct\nimport ctapp/g' cloned_reactor.py

