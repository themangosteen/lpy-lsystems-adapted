import lpy
l = lpy.Lsystem('/home/niko/Documents/Studium/Semester6/Bachelor Thesis CG/addons/lpy adapted/models/test_lsystem1.lpy')
print("FILENAME: {}\n".format(l.filename))
print("NUMBER OF RULES: {}\n".format(l.nbTotalRules()))
print("CODE:\n{}\n".format(l.code()))
print("STRING:\n{}\n".format(l.__str__()))
print("AXIOM: {}".format(l.axiom))
print("DERIVATION RESULT:\n{}".format(l.derive()))
