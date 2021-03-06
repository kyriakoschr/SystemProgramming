Μάθημα: Προγραμματισμός Συστήματος
Ονοματεπώνυμο: Κυριάκος Χριστοδούλου
ΑΜ: 1115201300211

Τα αρχεία που παρέδωσα έχουν υλοποιημένες όλες τις συναρτήσεις της εκφώνησης και την προαιρετική "dump".
Η μεταγλώττιση γίνεται με τη χρήση του αρχείου "makefile" που παρέδωσα και η εφαρμογή τρέχει με την εντολή
./elegxos -b "number of buckets" -o "operationsfile".
Στη συνέχεια ο χρήστης πέρνει τον έλεγχο μέσω του stdin, από το οποίο δίνει εντολές.

Παραδοχή: Δεν πρόλαβα να βρω κάποιο τρόπο που να τερματίζει η εφαρμογή με CTRL-D.

Γενικό πλάνο υλοποίησης:
Αρχικά υλοποίησα μια λίστα για τους λογαριασμούς(κορυφές) και μια λίστα για τις συναλλαγές(ακμές) του γράφου. Αφού ήμουν σίγουρος ότι λειτουργούν απόλυτα οι λειτουργίες εισαγωγής και διαγραφής με απλά δεδομένα(αριθμούς αντί για αντικείμενα), τότε τις "ένωσα" αναθέτοντας στην κλάση Λογαριασμός 2 δείκτες σε λίστες Συναλλαγών, μία για τις εισερχόμενες και μία για τις εξερχόμενες. Ακολούθως δημιούργησα τις κλάσεις HashTable και HashTableCell. Η πρώτη αποτελείται από ενα πίνακα απο ανικείμενα της δεύτερης, ενώ η δεύτερη απο μία λίστα Λογαριασμών. Έτσι αποφάσισα οι συναρτήσεις που μου ζητήθηκε να υλοποιήσω να αποτελούν συναρτήσεις-μέλη της κλάσης HashTable και αναλόγως να καλούναι συναρτήσεις άλλων κλάσεων για την επίτευξη του στόχου τους. Τον κάθε λογαριασμό τον βρίσκεις μέσα στο HashTableCell με αριθμό ίσο με το υπόλοιπο της διαίρεσης Ν/Β (ώπου Ν ο αριθμός λογαριασμού και Β το πλήθος τον κελλιών του πίνακα).

Διεπαφή με τις συναρτήσεις:
Επειδή δεν μου είχε μείνει όσος χρόνος θα μου χρειαζόταν για την διεπαφή του χρήστη με τις συναρτήσεις(main.cpp), μετά από λίγο ψάξιμο βρήκα ένα αλγόριθμο "σπασίματος" ενός string σε λέξεις στην ακόλουθη σελίδα:
http://www.sbin.org/doc/HOWTO/C++Programming-HOWTO-7.html
Με αυτόν δημιούργησα τη συνάρτηση tokenize, η οποία βάζει τις λέξεις του string σε μία λίστα(χρησιμοποίησα δική μου λίστα "strlist" αντί να κάνω χρήση του vector που προτίνει το εν λόγω site) και από εκεί τις παίρνω για να τρέξω τις συναρτήσεις του HashTable.

Παραδοχές:
1)Αρχικά αντί να δημιουργήσω ΔΥΟ λίστες(λογαριασμοί,ακμές), είχα σκεφτεί να δημιουργήσω ΜΙΑ γενική λίστα και να την χρησιμοποιήσω σε όλες τις περιπτώσεις αλλά το μετάνιωσα επειδή άργησα να ξεκινήσω την εργασία και ήμουν πιο σίγουρος με την παρούσα υλοποίηση.
2)Αν όμως η υλοποίησή μου ήταν όπως περιγράφω στην παραδοχή 2, τότε είχα στο μυαλό μου αντί για κάθε ακμή να δεσμέυεται μνήμη τόσο στο εξερχόμενο του αποστολέα όσο και στο εισερχόμενο του δέκτη, η λίστα εισερχομένων να είχε περιεχόμενο ένα δείκτη στην εξερχόμενη ακμή. Αυτο θα ήταν εξοικονόμηση χώρου αλλά επειδή ήθελε εξ'ολοκλήρου νέα λίστα μου φάνηκε πιο χρονοβόρο.
3)Όταν τρέξει με το μικρό dataset είναι μια χαρά. Όταν τρέξει με το 100 υπάρχει πρόβλημα το οποίο δυστυχώς δεν κατάφερα να επιλύσω.
