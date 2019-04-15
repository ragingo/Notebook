
;; C:\Users\shota_sakuragi\Downloads\IronScheme\IronScheme.Console-v4.exe heart_rotation6.ss

(import
    (rnrs)
    (ironscheme clr)
    (ironscheme clr dynamic)
    (ironscheme clr reflection)
)


(clr-reference System)
(clr-reference mscorlib)
(clr-using System)
(clr-using System.Reflection)
(clr-using System.Threading)

#|
(define (get-prop-val instance prop args)
    (property-get-value
        (clr-call Type GetProperty (clr-call Object GetType instance) prop)
        instance
        (list->vector args)
    )
)
(define (set-prop-val instance prop val args)
    (property-set-value
        (clr-call Type GetProperty (clr-call Object GetType instance) prop)
        instance
        val
        (list->vector args)
    )
)
|#

(begin
    (define xlWBATWorksheet -4167)

    (define xlAppType (clr-static-call Type GetTypeFromProgID "Excel.Application"))
    (define xlApp (clr-static-call Activator CreateInstance (clr-cast Type xlAppType)))
    (property-set-value (clr-call Type GetProperty (clr-call Object GetType xlApp) "Visible") xlApp '() #t)
    ;(set-prop-val (xlApp "Visible" #t '()))

    (define xlBooks (clr-dynamic xlApp Workbooks))

    ;(define xlBook (clr-dynamic xlBooks Add))
    ;(define xlBook (clr-call MethodBase Invoke (clr-call Type GetMethod (clr-call Object GetType xlBooks) "Add") xlBooks (clr-cast Object xlWBATWorksheet) ) )
    #|
    (define xlBook
        (method-invoke
            (clr-call Type GetMethod (clr-call Object GetType xlBooks) "Add")
            xlBooks
            xlWBATWorksheet)
    )
    |#
    

    (clr-static-call Thread Sleep 3000)
)
