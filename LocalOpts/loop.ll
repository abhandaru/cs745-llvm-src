; ModuleID = 'loop.bc'
target datalayout = "e-p:32:32:32-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:32:64-f32:32:32-f64:32:64-v64:64:64-v128:128:128-a0:0:64-f80:32:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

@g = common global i32 0, align 4

; Function Attrs: nounwind
define i32 @g_incr(i32 %c) #0 {
entry:
  %0 = load i32* @g, align 4, !tbaa !1
  %add = add nsw i32 %0, %c
  store i32 %add, i32* @g, align 4, !tbaa !1
  ret i32 %add
}

; Function Attrs: nounwind
define i32 @loop(i32 %a, i32 %b, i32 %c) #0 {
entry:
  %cmp2 = icmp sgt i32 %b, %a
  %0 = load i32* @g, align 4, !tbaa !1
  br i1 %cmp2, label %for.body.lr.ph, label %for.end

for.body.lr.ph:                                   ; preds = %entry
  %1 = sub i32 %b, %a
  %2 = mul i32 %1, %c
  %3 = add i32 %0, %2
  store i32 %3, i32* @g, align 4, !tbaa !1
  br label %for.end

for.end:                                          ; preds = %for.body.lr.ph, %entry
  %.lcssa = phi i32 [ %3, %for.body.lr.ph ], [ %0, %entry ]
  ret i32 %.lcssa
}

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
!1 = metadata !{metadata !2, metadata !2, i64 0}
!2 = metadata !{metadata !"int", metadata !3, i64 0}
!3 = metadata !{metadata !"omnipotent char", metadata !4, i64 0}
!4 = metadata !{metadata !"Simple C/C++ TBAA"}
