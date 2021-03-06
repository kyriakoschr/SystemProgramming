Μάθημα: Προγραμματισμός Συστήματος
Ονοματεπώνυμο: Κυριάκος Χριστοδούλου
ΑΜ: 1115201300211

Τα αρχεία που παρέδωσα έχουν υλοποιημένα όλα τα ζητούμενα της άσλησης 3 και δουλέυουν άψογα.

Με τη χρήση της εντολής make δημιουργούνται τα 2 εκτελέσιμα (server & client).

Αρχικά καλείται ο server ο οποίος δημιουργεί όσα worker threads πάρει σαν όρισμα και ένα μόνο master thread. Το master thread "ακούει" στο port για
συνδέσεις και κάθε σύνδεση που αποδέχεται την εισάγει στο queue που υπάρχει. Από εκείνο το queue παίρνουν τα worker threads όταν είναι διαθέσιμα
συνδέσεις μέσα από τις οποίες επικοινωνούν με τον εκάστοτε client κάθε φορά.

Η πιο πάνω διαδικασία (από την αποδοχή από το master thread μέχρι και την άντληση από ένα worker) είναι βασισμένη στο παράδειγμα των σημειώσεων
μόνο που σε αυτή τη περίπτωση χρειάστηκε να χρησιμοποιήσω επιπλέον mutexes και condition variables αφού ο writer είναι ένας (master γράφει στο queue)
και οι readers είναι πολλοί (κάθε worker που παίρνει ένα socket από το queue).

Εκτός από τα πιο πάνω, στον server αρχικοποιώ και ένα hash table (τον ίδιο με της πρώτης εργασίας) στον οποίο περνάνε οι λογαριασμοί και οι μεταφορές
χρημάτων.

Κάθε worker thread είναι σε ένα ατέρμον βρόγχο ώστε να πάρει από το queue ένα socket και να επικοινωνήσει με τον client. Όταν γίνει αυτό δέχεται την
πρώτη εντολή που στάληκε αφού πρώτα δεχθεί το μήκος της. Την επεξεργάζεται και την εκτελεί ανάλογα με το αν πληρεί τους συνατκτικούς κανόνες. Τότε
στέλνει στον client πίσω το αποτέλεσμα αφού και πάλι πρώτα στείλει το μήκος του. Τότε περιμένει την επόμενη εντολή από αυτόν. Αν έρθει το μήνυμα
EXITING τότε σημαίνει ότι πρέπει να κλείσει το socket και να μπει ξανά στη διαδικασία εύρεσης νέου cient αφού ο προηγούμενος τελέιωσε με τις
εργασίες του.

Στην πλευρά του client τώρα, τα πράγματα είναι πιο απλά. Στέλνει, αρχικά, αίτημα σύνδεσης στον server και ακολούθως ανοίγει και διαβάζει το αρχείο
εντολών τις οποίες και στέλνει στον server. Αν έρθει εντολή sleep εκτελείται τοπικά, όπως προφανώς και η exit. Αφού εκτελέσει όλες τις εντολές
του αρχείου και δεν τερματίσει λόγω κάποιου exit, τότε ο έλεγχος περνά στον χρήστη ο οποίος στέλνει εντολές από το terminal. Της ανταλλαγής
μηνυμάτων με τον server, όπως είπα και πριν, προηγείται το μέγεθος του μηνύματος.

Παραδοχή: Το μέγεθος του hash table ορίζεται στατικά με μία define htsize στο header file hasht.h. Το ίδιο και οι κλειδωνιές.
